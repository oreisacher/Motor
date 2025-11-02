//
// Created by Orik on 28/10/2025.
//

#include "../include/OrbitCamera.h"

#include <iostream>
#include <ostream>
#include <core/InputManager.h>
#include <core/SceneNode.h>
#include <glm/glm.hpp>

void OrbitCamera::init() {
}

void OrbitCamera::update(float deltaTime) {
    // Mouse Look
    auto mousePos = Motor::Input.getMousePosition();

    float xDelta = prevMousePosition.first - mousePos.first;
    float yDelta = prevMousePosition.second - mousePos.second;

    prevMousePosition = mousePos;

    yaw += xDelta * deltaTime * MouseSensitivity;
    pitch -= yDelta * deltaTime * MouseSensitivity;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    auto scrollDelta = Motor::Input.getScrollDelta();
    radius -= scrollDelta.second * 0.5;
    radius = glm::clamp(radius, 0.5f, 5.0f);

    glm::vec3 position;
    position.x = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    position.y = radius * sin(glm::radians(pitch));
    position.z = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));

    glm::vec3 camPos = target->getGlobalPosition() + position;

    node->setPosition(camPos);
    glm::vec3 forward = glm::normalize(target->getGlobalPosition() - camPos);
    node->setRotation(glm::quatLookAt(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
}