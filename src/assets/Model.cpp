//
// Created by Orik on 09/10/2025.
//

#include "assets/Model.h"
#include "assets/ModelInstance.h"

#include <ranges>
#include <memory>

using namespace Motor;

Model::~Model() {
    // Delete model node tree
    deleteNode(root);
}

std::unique_ptr<ModelInstance> Model::instantiate() const {
    return std::make_unique<ModelInstance>(this);
}

Model::Node* Model::getRoot() const { return root; }

std::vector<Mesh*> Model::getMeshes() const {
    std::vector<Mesh*> result;

    for (auto& mesh : meshes)
        result.push_back(mesh.get());

    return result;
}

Mesh* Model::getMesh(int index) const {
    if (index < 0 || index >= meshes.size()) {
        LOG_ERROR("Mesh index out of range");
        return nullptr;
    }
    return meshes[index].get();
}

std::vector<Model::Node*> Model::getMeshNodes() const { return meshNodes; }

int Model::getMeshCount() const { return meshes.size(); }

// Material* Model::getMaterial(const int meshIndex) const {
//     if (meshIndex < 0 || meshIndex >= meshes.size()) {
//         LOG_ERROR("Mesh index out of range")
//         return nullptr;
//     }
//
//     return meshes[meshIndex]->getMaterial();
// }
//
// void Model::setMaterial(const int meshIndex, Material* material) const {
//     if (meshIndex < 0 || meshIndex >= meshes.size()) {
//         LOG_ERROR("Mesh index out of range")
//         return;
//     }
//
//     meshes[meshIndex]->setMaterial(material);
// }

int Model::getAnimationCount() const {
    return animations.size();
}

Animation* Model::getAnimation(const std::string &name) const {
    auto it = animations.find(name);
    if (it == animations.end()) {
        LOG_ERROR("Animation not found: " + name);
        return nullptr;
    }
    return it->second.get();
}

std::vector<std::string> Model::getAnimationNames() const {
    std::vector<std::string> result;

    for (const auto &key: animations | std::views::keys)
        result.push_back(key);

    return result;
}

bool Model::isSkinned() const {
    return boneOffsetMatrices.size() > 0;
}

int Model::getBone(const std::string &name) const {
    auto it = boneLookup.find(name);
    if (it == boneLookup.end())
        return -1;
    return it->second;
}

std::vector<glm::mat4> Model::getBoneTransforms() const { return boneOffsetMatrices; }

glm::mat4 Model::getBoneTransform(int index) const {
    if (index < 0 || index >= boneOffsetMatrices.size()) {
        LOG_ERROR("Bone index out of range")
        return glm::mat4(1.0f);
    }
    return boneOffsetMatrices[index];
}

const char* Model::getType() const { return "Model"; }

void Model::deleteNode(Node *node) {
    for (auto &child : node->children)
        deleteNode(child);

    delete node;
}
