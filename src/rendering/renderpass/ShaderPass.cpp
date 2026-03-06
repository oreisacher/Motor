//
// Created by Orik on 25/10/2025.
//

#include <motor/rendering/renderpass/ShaderPass.h>
#include <motor/assets/AssetManager.h>
#include <motor/rendering/WindowManager.h>
#include <motor/utils/ShaderUniformHelper.h>

#include "glad/gl.h"

using namespace Motor;

ShaderPass::ShaderPass(const std::string &inTexture, const std::string &outTexture, std::shared_ptr<ShaderInstance> shader) {
    this->shader = shader;
    this->inTexture = inTexture;
    this->outTexture = outTexture;

    glGenVertexArrays(1, &vao);

    auto activeWindow = WindowManager::GetActiveWindow();
    updateSize(activeWindow->getWidth(), activeWindow->getHeight());
}

void ShaderPass::execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glDisable(GL_DEPTH_TEST);

    auto activeWindow = WindowManager::GetActiveWindow();
    glViewport(0, 0, activeWindow->getWidth(), activeWindow->getHeight());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GPUShader* gpuShader = resourceManager.loadShader(shader->getShader());
    glUseProgram(gpuShader->id);

    GLuint sceneColor = registry.get(inTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneColor);
    setUniformInt(gpuShader->id, "renderTexture", 0);

    // Set additional uniforms
    for (auto &uniform : shader->getUniforms()) {
        if (uniform.second.type() == typeid(float))
            setUniformFloat(gpuShader->id, uniform.first, std::any_cast<float>(uniform.second));
        if (uniform.second.type() == typeid(int))
            setUniformInt(gpuShader->id, uniform.first, std::any_cast<int>(uniform.second));
        if (uniform.second.type() == typeid(glm::vec3))
            setUniformVec3(gpuShader->id, uniform.first, std::any_cast<glm::vec3>(uniform.second));
        if (uniform.second.type() == typeid(bool))
            setUniformBool(gpuShader->id, uniform.first, std::any_cast<bool>(uniform.second));
        // TODO - Add remaining
    }

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glEnable(GL_DEPTH_TEST);

    registry.set(outTexture, fboTexture);
}

void ShaderPass::updateSize(int width, int height) {
    if (fbo) glDeleteFramebuffers(1, &fbo);
    if (fboTexture) glDeleteTextures(1, &fboTexture);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
