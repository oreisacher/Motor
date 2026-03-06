//
// Created by Orik on 09/10/2025.
//

#ifndef MOTOR_MODELLOADER_H
#define MOTOR_MODELLOADER_H

#include <assets/Model.h>
#include <assimp/scene.h>

#include "AssetLoader.h"

namespace Motor {
    class ModelLoader : public AssetLoader {
    public:
        std::unique_ptr<Asset> load(const std::string& path) override;
        void registerDefaultAssets() override;

    private:
        Model::Node* processNode(const aiNode* node, const aiScene* scene);
        std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene) const;
        void loadTextures(const aiMaterial* material, aiTextureType type, const std::string &sampler, Material* meshMaterial) const;
        void loadAnimations(const aiScene *scene) const;
        bool hasAnimation(const std::string &nodeName);

        // void calculateBindPoseTransforms(Model::Node *node, const glm::mat4 &parentTransform);

        // Temporal data for latest model loading process
        Model* model;
        std::string currentWorkingDirectory;
    };
};

#endif //MOTOR_MODELLOADER_H