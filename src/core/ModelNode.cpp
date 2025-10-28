//
// Created by Orik on 09/10/2025.
//

#include "core/ModelNode.h"

using namespace Motor;

ModelNode::ModelNode() = default;
ModelNode::ModelNode(const std::string &name) : SceneNode(name) { }
ModelNode::ModelNode(const std::string &name, ModelInstance* model) : SceneNode(name) {
    this->model = model;
}

ModelInstance* ModelNode::getModelInstance() const { return model; }
void ModelNode::setModelInstance(ModelInstance* model) { this->model = model; }

std::string ModelNode::toString() const {
    return "ModelNode (" + name + ") - Model: " + (model ? model->getModel()->getName(): "null");
}

std::string ModelNode::getType() const { return "ModelNode"; }
