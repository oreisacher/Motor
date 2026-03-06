//
// Created by Orik on 24/10/2025.
//

#ifndef MOTOR_MODELINSTANCE_H
#define MOTOR_MODELINSTANCE_H

#include "assets/Model.h"

namespace Motor {
    class ModelInstance {
    public:
        struct InstanceData {
            glm::mat4 animationTransform;
            glm::mat4 globalTransform;
        };

        ModelInstance(const Model* model);

        const Model* getModel() const;

        Material* getMaterialForMesh(int index);
        Material* getMaterialForMesh(const std::string &name);
        void setMaterialForMesh(int index, Material* material);
        void setMaterialForMesh(const std::string &name, Material* material);

        std::map<std::string, InstanceData> instanceData;
        std::vector<glm::mat4> boneTransforms;
    private:
        const Model *model = nullptr;

        std::map<std::string, Material*> materials;

        void processModelTransforms(const Model::Node* node, const glm::mat4 &parentTransform);
    };
}

#endif //MOTOR_MODELINSTANCE_H