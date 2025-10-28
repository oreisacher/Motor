//
// Created by Orik on 18/09/2025.
//

#include "core/Scene.h"

#include <iostream>

#include "assets/AssetManager.h"

using namespace Motor;

Scene::Scene(const std::string &name) :
    name(name),
    root(std::make_unique<SceneNode>("root")),
    skybox(AssetManager::getInstance()->load<Material>("motor://materials/skybox.material")) {
}

SceneNode* Scene::getRoot() const { return root.get(); }

void Scene::printTree() {
    std::cout << "Scene '" << name << "'" << std::endl;
    root->printTree(0, "  ", true);
}

void Scene::init() const {
    root->init();
}

void Scene::update(float deltaTime) const {
    root->update(deltaTime);
}

void Scene::updateGlobalTransforms() const {
    root->updateGlobalTransform(glm::mat4(1.0f));
}

void Scene::setSkyboxColor(glm::vec3 color) const { skybox->setColor(color); }
glm::vec3 Scene::getSkyboxColor() const { return skybox->getColor(); }

void Scene::setSkyboxTexture(Texture *texture) const { skybox->addTexture("skybox", texture); }
