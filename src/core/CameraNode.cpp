//
// Created by Orik on 12/10/2025.
//

#include "core/CameraNode.h"

using namespace Motor;

CameraNode::CameraNode() = default;

CameraNode::CameraNode(const std::string &name, float param, float aspect, float near, float far, int orthographic)
    : SceneNode(name), aspect(aspect), nearPlane(near), farPlane(far), orthographic(orthographic) {
    if (orthographic)
        orthoSize = param;
    else
        fov = param;

    updateProjectionMatrix();

    // Check if camera is only camera and therefore active
    count++;
    active = count == 1;
}

float CameraNode::getFOV() const { return fov; }
void CameraNode::setFOV(float fov) {
    this->fov = fov;
    updateProjectionMatrix();
}

float CameraNode::getAspect() const { return aspect; }
void CameraNode::setAspect(float aspect) {
    this->aspect = aspect;
    updateProjectionMatrix();
}

float CameraNode::getNear() const { return nearPlane; }
void CameraNode::setNear(float near) {
    this->nearPlane = near;
    updateProjectionMatrix();
}

float CameraNode::getFar() const { return farPlane; }
void CameraNode::setFar(float far) {
    this->farPlane = far;
    updateProjectionMatrix();
}

bool CameraNode::isOrthographic() const { return orthographic; }
void CameraNode::setOrthographic(bool ortho) {
    this->orthographic = ortho;
    updateProjectionMatrix();
}

float CameraNode::getOrthographicSize() const { return orthoSize; }
void CameraNode::setOrthographicSize(float size) {
    this->orthoSize = size;
    updateProjectionMatrix();
}

bool CameraNode::isActive() const { return active; }
void CameraNode::setActive(bool active) { this->active = active; }

glm::mat4 CameraNode::getProjectionMatrix() const { return projectionMatrix; }

void CameraNode::updateProjectionMatrix() {
    if (orthographic)
        projectionMatrix = glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, nearPlane, farPlane);
    else
        projectionMatrix = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

std::string CameraNode::toString() const {
    return "CameraNode (" + name + ")";
}

std::string CameraNode::getType() const { return "CameraNode"; }
