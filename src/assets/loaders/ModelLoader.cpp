//
// Created by Orik on 09/10/2025.
//

#include <motor/assets/loaders/ModelLoader.h>
#include <motor/utils/AssimpConverts.h>
#include <motor/SkyboxCubeData.h>
#include <motor/assets/BenzinAssimpAdapter.h>
#include <motor/assets/Shader.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <format>

#include "stb_image.h"

using namespace Motor;

std::unique_ptr<Asset> ModelLoader::load(const std::string &path) {
    model = new Model;

    std::filesystem::path fsPath(path);
    model->name = fsPath.stem().string();
    model->path = path;

    currentWorkingDirectory = fsPath.parent_path().generic_string();

    LOG_INFO("Loading model '" + model->name + "' on path '" + path + "'")

    auto rawData = AssetManager::getInstance()->readBytes(path);
    if (rawData.empty())
        return nullptr;

    // Parse raw data with assimp model loader
    Assimp::Importer importer;
    importer.SetIOHandler(new BenzinIOSystem(AssetManager::getInstance()->getLoader(), currentWorkingDirectory));
    const aiScene* scene = importer.ReadFileFromMemory(
        rawData.data(),
        rawData.size(),
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR(importer.GetErrorString())
        return nullptr;
    }

    // Build node tree, construct meshes and load textures
    model->root = processNode(scene->mRootNode, scene);

    loadAnimations(scene);

    return std::unique_ptr<Model>(model);
}

void ModelLoader::registerDefaultAssets() {
    // Skybox Cube
    auto skybox = new Model();
    skybox->name = "skyboxCube";
    skybox->path = "motor://models/skybox";

    auto skyboxNode = new Model::Node();
    skyboxNode->name = "skybox";
    skyboxNode->localTransform = glm::mat4(1.0f);
    skybox->root = skyboxNode;

    auto skyboxMesh = std::make_unique<Mesh>(skyboxVertices, skyboxIndices);
    skybox->defaultMaterials[skyboxMesh->getName()] = AssetManager::getInstance()->load<Material>("motor://materials/skybox.material");
    skybox->meshes.push_back(std::move(skyboxMesh));

    AssetManager::getInstance()->registerAsset(skybox->path, skybox);
}

Model::Node* ModelLoader::processNode(const aiNode *node, const aiScene *scene) {
    auto currentNode = new Model::Node();
    currentNode->name = node->mName.C_Str();
    currentNode->localTransform = aiMatrix4x4ToGlmMat4(node->mTransformation);

    if (node->mNumMeshes > 0)
        model->meshNodes.push_back(currentNode);

    // Process meshes on node
    for (unsigned int i = 0 ; i < node->mNumMeshes ; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(processMesh(mesh, scene));

        currentNode->meshes.push_back(model->meshes.size() - 1);
    }

    // Call process for each child node
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        currentNode->children.push_back(processNode(node->mChildren[i], scene));
    }

    return currentNode;
}

std::unique_ptr<Mesh> ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) const {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex {
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z)
        };

        // Add normals if existing
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        // Add texture coords if existing
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        if (mesh->HasTangentsAndBitangents()) {
            vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process Vertex Bone Data
    for (unsigned int i = 0 ; i < mesh->mNumBones ; i++) {
        aiBone* bone = mesh->mBones[i];
        std::string boneName = bone->mName.C_Str();

        // Add new bone if not existing yet
        if (!model->boneLookup.contains(boneName)) {
            // Map Bone Name to current last index of array
            model->boneLookup[boneName] = model->boneOffsetMatrices.size();

            // Add new bone transform
            model->boneOffsetMatrices.push_back(aiMatrix4x4ToGlmMat4(bone->mOffsetMatrix));
        }

        for (unsigned int j = 0; j < bone->mNumWeights; j++) {
            aiVertexWeight weight = bone->mWeights[j];
            if (weight.mWeight != 0.0f) {
                vertices[weight.mVertexId].addBoneData(model->boneLookup[boneName], weight.mWeight);
            }
        }
    }

    // Process material
    Material* meshMaterial;

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::string materialPath = model->path + "/" + material->GetName().C_Str();

        // Check if material is already loaded
        if (AssetManager::getInstance()->isLoaded(materialPath)) {
            meshMaterial = AssetManager::getInstance()->load<Material>(materialPath);
            model->defaultMaterials[mesh->mName.C_Str()] = meshMaterial;
        } else {
            // Create new material
            meshMaterial = new Material();
            meshMaterial->name = material->GetName().C_Str();
            meshMaterial->path = materialPath;

            // Load defaultLit shader
            auto defaultLit = AssetManager::getInstance()->load<Shader>("motor://shaders/defaultLit.shader");
            meshMaterial->shader = defaultLit;
            meshMaterial->shaderInstance = defaultLit->getDefaultInstance();

            // Base Color
            aiColor4D result;
            if (material->Get(AI_MATKEY_COLOR_DIFFUSE, result) == AI_SUCCESS) {
                meshMaterial->baseColor = glm::vec3(result.r, result.g, result.b);
            }

            // Load textures
            stbi_set_flip_vertically_on_load(false);
            loadTextures(material, aiTextureType_DIFFUSE, "diffuse", meshMaterial);

            loadTextures(material, aiTextureType_SPECULAR, "metallic", meshMaterial);
            loadTextures(material, aiTextureType_METALNESS, "metallic", meshMaterial);

            loadTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "roughness", meshMaterial);
            loadTextures(material, aiTextureType_SHININESS, "roughness", meshMaterial);

            loadTextures(material, aiTextureType_AMBIENT_OCCLUSION, "ao", meshMaterial);
            loadTextures(material, aiTextureType_LIGHTMAP, "ao", meshMaterial);

            loadTextures(material, aiTextureType_NORMALS, "normal", meshMaterial);
            loadTextures(material, aiTextureType_HEIGHT, "normal", meshMaterial);
            stbi_set_flip_vertically_on_load(true);

            model->defaultMaterials[mesh->mName.C_Str()] = meshMaterial;
            AssetManager::getInstance()->registerAsset(materialPath, meshMaterial);
        }
    }

    // Setup mesh
    auto modelMesh = std::make_unique<Mesh>(vertices, indices);
    modelMesh->name = mesh->mName.C_Str();
    modelMesh->path = model->name + "/" + modelMesh->name;

    return modelMesh;
}

void ModelLoader::loadTextures(const aiMaterial* material, aiTextureType type, const std::string &sampler, Material* meshMaterial) const {
    for (unsigned int i = 0 ; i < material->GetTextureCount(type) ; i++) {
        aiString str;
        material->GetTexture(type, i, &str);

        auto texture = AssetManager::getInstance()->load<Texture>(currentWorkingDirectory + "/" + str.C_Str());

        if (!texture)
            continue;

        switch (type) {
            case aiTextureType_DIFFUSE: texture->setTextureType(Texture::DIFFUSE); break;
            case aiTextureType_SPECULAR: texture->setTextureType(Texture::METALLIC); break;
            case aiTextureType_METALNESS: texture->setTextureType(Texture::METALLIC); break;
            case aiTextureType_DIFFUSE_ROUGHNESS: texture->setTextureType(Texture::ROUGHNESS); break;
            case aiTextureType_SHININESS: texture->setTextureType(Texture::ROUGHNESS); break;
            case aiTextureType_AMBIENT_OCCLUSION: texture->setTextureType(Texture::AO); break;
            case aiTextureType_LIGHTMAP: texture->setTextureType(Texture::AO); break;
            case aiTextureType_NORMALS: texture->setTextureType(Texture::NORMAL); break;
            case aiTextureType_HEIGHT: texture->setTextureType(Texture::NORMAL); break;
            default: break;
        }

        texture->setFilter(Texture::Filter::LINEAR);

        meshMaterial->addTexture(sampler + std::to_string(i), texture);
    }
}

void ModelLoader::loadAnimations(const aiScene *scene) const {
    for (unsigned int i = 0 ; i < scene->mNumAnimations ; i++) {
        std::string animationName = scene->mAnimations[i]->mName.length != 0 ? scene->mAnimations[i]->mName.C_Str() : std::format("default{}", i);

        auto animation = std::make_unique<Animation>(animationName);
        animation->duration = scene->mAnimations[i]->mDuration;
        animation->ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;

        // Go through all animated channels (e.g. bones) and extract keyframes
        for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
            aiNodeAnim* channel = scene->mAnimations[i]->mChannels[j];

            // Parse Position Keys
            for (int pk = 0 ; pk < channel->mNumPositionKeys ; pk++) {
                animation->addKeyframe<glm::vec3>(
                    channel->mNodeName.C_Str(),
                    POSITION,
                    channel->mPositionKeys[pk].mTime,
                    aiVec3ToGlmVec3(channel->mPositionKeys[pk].mValue)
                );
            }

            // Parse Rotation Keys
            for (int rk = 0 ; rk < channel->mNumRotationKeys ; rk++) {
                animation->addKeyframe<glm::quat>(
                    channel->mNodeName.C_Str(),
                    ROTATION,
                    channel->mRotationKeys[rk].mTime,
                    aiQuatToGlmQuat(channel->mRotationKeys[rk].mValue)
                );
            }

            // Parse Scale Keys
            for (int sk = 0 ; sk < channel->mNumScalingKeys ; sk++) {
                animation->addKeyframe<glm::vec3>(
                    channel->mNodeName.C_Str(),
                    SCALE,
                    channel->mScalingKeys[sk].mTime,
                    aiVec3ToGlmVec3(channel->mScalingKeys[sk].mValue)
                );
            }
        }

        model->animations[animationName] = std::move(animation);
    }
}
