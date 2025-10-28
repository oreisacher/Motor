//
// Created by Orik on 18/09/2025.
//

#include "core/SceneNode.h"

#include <format>
#include <iostream>
#include <ostream>
#include <components/Component.h>
#include <core/Scene.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utils/Logger.h>

using namespace Motor;

SceneNode::SceneNode() :
    name("New Node"),
    position(0.0f),
    rotation(0.0f, 0.0f, 0.0f, 0.0f),
    scale(1.0f) {
    updateLocalTransform();
}

SceneNode::SceneNode(std::string name) :
    name(name),
    position(0.0f),
    rotation(0.0f, 0.0f, 0.0f, 0.0f),
    scale(1.0f) {
    updateLocalTransform();
}

std::string SceneNode::getName() const { return name; }
void SceneNode::setName(const std::string &name) { this->name = name; }

std::vector<Component*> SceneNode::getComponents() const {
    std::vector<Component*> result;

    for (auto& component : components)
        result.push_back(component.get());

    return result;
}

void SceneNode::addComponent(std::unique_ptr<Component> component) {
    if (component->node) {
        LOG_ERROR("Component is already attached to a node")
        return;
    }

    component->node = this;
    components.push_back(std::move(component));
}

void SceneNode::removeComponent(Component* component) {
    std::erase_if(components, [&](const std::unique_ptr<Component>& c) {
        if (c.get() == component) {
            return true;
        }
        return false;
    });
}

SceneNode* SceneNode::getParent() { return parent; }

int SceneNode::getChildCount() const { return children.size(); }

std::vector<SceneNode*> SceneNode::getChildren() {
    std::vector<SceneNode*> result;
    result.reserve(children.size());

    for (auto &child : children)
        result.push_back(child.get());

    return result;
}

SceneNode* SceneNode::getChild(const std::string &name) {
    for (auto &child : children)
        if (child->getName() == name)
            return child.get();
    return nullptr;
}

SceneNode* SceneNode::getChild(int index) {
    return children[index].get();
}

void SceneNode::addChild(SceneNode* child) {
    child->parent = this;
    children.push_back(std::unique_ptr<SceneNode>(child));
}

void SceneNode::removeChild(const SceneNode* child) {
    std::erase_if(children, [&](const std::unique_ptr<SceneNode> &node) {
       return node.get() == child;
    });
}

void SceneNode::removeChild(int index) {
    if (index < 0 || index >= children.size())
        return;
    children.erase(children.begin() + index);
}

glm::vec3 SceneNode::getPosition() const { return position; }
glm::vec3 SceneNode::getGlobalPosition() const { return globalPosition; }

void SceneNode::setPosition(const glm::vec3 &position) { this->position = position; updateLocalTransform(); }

glm::quat SceneNode::getRotation() const { return rotation; }
glm::quat SceneNode::getGlobalRotation() const { return globalRotation; }

void SceneNode::setRotation(const glm::quat &rotation) { this->rotation = rotation; updateLocalTransform(); }
void SceneNode::setRotation(const glm::vec3 &rotation) { this->rotation = glm::quat(glm::radians(rotation)); updateLocalTransform(); }

glm::vec3 SceneNode::getScale() const { return scale; }
glm::vec3 SceneNode::getGlobalScale() const { return globalScale; }

void SceneNode::setScale(const glm::vec3 &scale) { this->scale = scale; updateLocalTransform(); }

glm::mat4 SceneNode::getLocalTransform() const { return localTransform; }
glm::mat4 SceneNode::getGlobalTransform() const { return globalTransform; }

void SceneNode::init() {
    for (auto& component : components)
        component->init();

    for (auto& child : children)
        child->init();
}

void SceneNode::update(float deltaTime) {
    for (auto& component : components)
        if (component->isEnabled())
            component->update(deltaTime);

    for (auto& child : children)
        child->update(deltaTime);
}

void SceneNode::updateGlobalTransform(const glm::mat4 &parentGlobalTransform) {
    globalTransform = parentGlobalTransform * localTransform;

    if (parent) {
        globalPosition = parent->getGlobalPosition() + parent->getGlobalRotation() * (parent->getGlobalScale() * position);
        globalRotation = parent->getGlobalRotation() * rotation;
        globalScale = parent->getGlobalScale() * scale;
    } else {
        globalPosition = position;
        globalRotation = rotation;
        globalScale = scale;
    }

    for (const auto& child : children) {
        child->updateGlobalTransform(globalTransform);
    }
}

std::string SceneNode::toString() const {
    return std::format("SceneNode ({}) - Position: ({:.2f}, {:.2f}, {:.2f}) - Global Position: ({:.2f}, {:.2f}, {:.2f})",
                       name, position.x, position.y, position.z, globalPosition.x, globalPosition.y, globalPosition.z);
}

void SceneNode::printTree(int depth, const std::string &prefix, bool isLast) const {
    std::cout << prefix;
    if (prefix.size() > 0)
        std::cout << (isLast ? "`-- " : "|-- ");
    std::cout << *this << std::endl;

    std::string childPrefix = prefix + (isLast ? "    " : "|   ");

    for (int i = 0 ; i < children.size(); i++) {
        children[i]->printTree(depth + 1, childPrefix, i == children.size() - 1);
    }
}

std::string SceneNode::getType() const { return "SceneNode"; }

void SceneNode::updateLocalTransform() {
    localTransform =
        glm::translate(glm::mat4(1.0f), this->position) *
        glm::mat4_cast(this->rotation) *
        glm::scale(glm::mat4(1.0f), this->scale);
}

namespace Motor {
    std::ostream& operator<<(std::ostream &os, const SceneNode &node) {
        os << node.toString();
        return os;
    }
}
