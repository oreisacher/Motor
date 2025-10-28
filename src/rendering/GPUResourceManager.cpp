//
// Created by Orik on 16/10/2025.
//

#include "rendering/GPUResourceManager.h"

#include <format>
#include <assets/Mesh.h>
#include <glad/gl.h>

#include "glm/gtc/type_ptr.hpp"

using namespace Motor;

GPUShader* GPUResourceManager::loadShader(const Shader *shader) {
    // Check if shader is already loaded
    auto it = loaded.find(shader->getId());
    if (it != loaded.end()) {
        return static_cast<GPUShader*>(it->second.get());
    }

    // Compile shader
    return compileShader(shader);
}

GPUTexture* GPUResourceManager::loadTexture(const Texture *texture) {
    // Check if texture is already loaded
    auto it = loaded.find(texture->getId());
    if (it != loaded.end()) {
        return static_cast<GPUTexture*>(it->second.get());
    }

    // Load texture
    return uploadTexture(texture);
}

GPUMesh * GPUResourceManager::loadMesh(const Mesh *mesh) {
    // Check if mesh is already loaded
    auto it = loaded.find(mesh->getId());
    if (it != loaded.end()) {
        return static_cast<GPUMesh*>(it->second.get());
    }

    // Load mesh
    return uploadMesh(mesh);
}

void GPUResourceManager::updateInstanceBuffer(GPUMesh *mesh, const std::vector<glm::mat4> &transforms) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh->instanceVBO);

    size_t dataSize = transforms.size() * sizeof(glm::mat4);

    if (mesh->instanceDataSize < dataSize) {
        mesh->instanceDataSize = dataSize;
        glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
    }

    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, dataSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(ptr, transforms.data(), dataSize);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

GPUShader* GPUResourceManager::compileShader(const Shader* shader) {
    unsigned int shaderProgram = glCreateProgram();
    std::vector<unsigned int> compiledShaders;

    // Compile all shader sources attached to shader
    for (auto& src : shader->getSources()) {
        unsigned int compiledSource = compileShaderSource(src);
        if (compiledSource == -1) {
            glDeleteProgram(shaderProgram);
            return nullptr;
        }

        compiledShaders.push_back(compiledSource);
        glAttachShader(shaderProgram, compiledSource);
    }

    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        LOG_ERROR(infoLog)
        glDeleteProgram(shaderProgram);
        return nullptr;
    }

    // Cleanup shaders
    for (const unsigned int shaderID : compiledShaders) {
        glDeleteShader(shaderID);
    }

    // Create GPUShader
    std::shared_ptr<GPUShader> gpuShader = std::make_shared<GPUShader>(shaderProgram);

    // Cache shader
    loaded[shader->getId()] = gpuShader;
    return gpuShader.get();
}

unsigned int GPUResourceManager::compileShaderSource(ShaderSource *source) {
    GLenum shaderType;
    switch (source->getShaderType()) {
        case ShaderSource::VERTEX: shaderType = GL_VERTEX_SHADER; break;
        case ShaderSource::FRAGMENT: shaderType = GL_FRAGMENT_SHADER; break;
        case ShaderSource::TESSELLATION_CONTROL: shaderType = GL_TESS_CONTROL_SHADER; break;
        case ShaderSource::TESSELLATION_EVAL: shaderType = GL_TESS_EVALUATION_SHADER; break;
        case ShaderSource::GEOMETRY: shaderType = GL_GEOMETRY_SHADER; break;
        default: LOG_ERROR("Unknown shader type"); return -1;
    }

    unsigned int shader = glCreateShader(shaderType);

    // Compile
    int success;
    const char* srcPtr = source->getData().c_str();
    glShaderSource(shader, 1, &srcPtr, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        LOG_ERROR(infoLog)
        return -1;
    }

    return shader;
}

GPUTexture* GPUResourceManager::uploadTexture(const Texture *texture) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Texture filters
    switch (texture->getFilter()) {
        case Texture::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case Texture::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
    }

    // Texture wrap
    switch (texture->getWrap()) {
        case Texture::REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case Texture::MIRRORED_REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case Texture::CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
    }


    // Generate texture
    unsigned int format;
    unsigned int internalFormat;

    switch (texture->getNrChannels()) {
        case 1:
            format = GL_RED;         // single channel (R)
            internalFormat = GL_R8;  // linear 8-bit channel
            break;
        case 3:
            format = GL_RGB;
            internalFormat = texture->isColor() ? GL_SRGB8 : GL_RGB8; // Use SRGB8 for albedo
            break;
        case 4:
            format = GL_RGBA;
            internalFormat = texture->isColor() ? GL_SRGB8_ALPHA8 : GL_RGBA8; // Use SRGB8_ALPHA8 for albedo
            break;
        default:
            std::cerr << "Unsupported channel count" << std::endl;
            break;
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        texture->getWidth(),
        texture->getHeight(),
        0,
        format,
        GL_UNSIGNED_BYTE,
        texture->getData()
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);


    // Cache texture
    std::shared_ptr<GPUTexture> gpuTexture = std::make_shared<GPUTexture>(textureID);
    loaded[texture->getId()] = gpuTexture;
    return gpuTexture.get();
}

GPUMesh* GPUResourceManager::uploadMesh(const Mesh *mesh) {
    if (!mesh) {
        LOG_ERROR("Mesh is nullptr")
        return nullptr;
    }

    unsigned int vao, vbo, ebo, instanceVBO;

    // VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertices().size() * sizeof(Vertex), mesh->getVertices().data(), GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndices().size() * sizeof(unsigned int), mesh->getIndices().data(), GL_STATIC_DRAW);

    // Vertex attributes
    GLsizei stride = sizeof(Vertex);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);

    // TexCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, TexCoord));
    glEnableVertexAttribArray(1);

    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);

    // Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(3);

    // Bitangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Bitangent));
    glEnableVertexAttribArray(4);

    // Joints
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Joints_0));
    glEnableVertexAttribArray(5);

    // Weights
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Weights_0));
    glEnableVertexAttribArray(6);

    // Instance VBO
    glm::mat4 identity = glm::mat4(1.0f);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(identity), GL_DYNAMIC_DRAW);

    // Instance Transform
    for (int i = 0 ; i < 4 ; i++) {
        glVertexAttribPointer(7 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * i));
        glEnableVertexAttribArray(7 + i);
        glVertexAttribDivisor(7 + i, 1);
    }

    glBindVertexArray(0);

    // Cache mesh
    std::shared_ptr<GPUMesh> gpuMesh = std::make_shared<GPUMesh>(vao, vbo, ebo, instanceVBO);
    loaded[mesh->getId()] = gpuMesh;
    return gpuMesh.get();
}
