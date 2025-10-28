//
// Created by Orik on 24/10/2025.
//

#include "assets/ModelInstance.h"
#include <assets/Model.h>

using namespace Motor;

ModelInstance::ModelInstance(const Model *model) : model(model) {
    boneTransforms.resize(model->getBoneTransforms().size());
    materials = model->defaultMaterials;

    processModelTransforms(model->getRoot(), glm::mat4(1.0f));
}

const Model* ModelInstance::getModel() const { return model; }

Material* ModelInstance::getMaterialForMesh(int index) {
    if (index < 0 || index >= model->getMeshes().size()) {
        LOG_ERROR("Mesh index out of range")
        return nullptr;
    }

    return materials[model->getMesh(index)->getName()];
}

Material* ModelInstance::getMaterialForMesh(const std::string &name) {
    return materials.contains(name) ? materials[name] : nullptr;
}

void ModelInstance::setMaterialForMesh(int index, Material *material) {
    if (index < 0 || index >= model->getMeshes().size()) {
        LOG_ERROR("Mesh index out of range")
        return;
    }

    materials[model->getMesh(index)->getName()] = material;
}

void ModelInstance::setMaterialForMesh(const std::string &name, Material *material) {
    for (int i = 0 ; i < model->getMeshes().size() ; i++) {
        if (model->getMesh(i)->getName() == name) {
            materials[model->getMesh(i)->getName()] = material;
        }
    }
}

void ModelInstance::processModelTransforms(const Model::Node* node, const glm::mat4 &parentTransform) {
    InstanceData data;

    data.animationTransform = node->localTransform;
    data.globalTransform = parentTransform * node->localTransform;

    int boneIndex = model->getBone(node->name);
    if (boneIndex >= 0 && boneIndex < boneTransforms.size()) {
        boneTransforms[boneIndex] = data.globalTransform * model->getBoneTransform(boneIndex);
    }

    instanceData[node->name] = data;

    for (auto &child : node->children)
        processModelTransforms(child, data.globalTransform);
}
