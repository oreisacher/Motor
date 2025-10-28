//
// Created by Orik on 20/10/2025.
//

#include "components/FreeCam.h"

#include <iostream>
#include <ostream>
#include <core/InputManager.h>
#include <core/SceneNode.h>
#include <glm/glm.hpp>

using namespace Motor;

void FreeCam::init() { }

void FreeCam::update(float deltaTime) {
    glm::mat4 transform = node->getGlobalTransform();

    const glm::vec3 front = -glm::vec3(transform[2]);
    const glm::vec3 right = glm::vec3(transform[0]);

    // WASD Movement
    float velocity = MovementSpeed * deltaTime;

    if (Input.isKeyPressed(Window::Key::W))
        node->setPosition(node->getPosition() + front * velocity);
    if (Input.isKeyPressed(Window::Key::S))
        node->setPosition(node->getPosition() - front * velocity);
    if (Input.isKeyPressed(Window::Key::A))
        node->setPosition(node->getPosition() - right * velocity);
    if (Input.isKeyPressed(Window::Key::D))
        node->setPosition(node->getPosition() + right * velocity);

    // Mouse Look
    auto mousePos = Input.getMousePosition();

    float xDelta = prevMousePosition.first - mousePos.first;
    float yDelta = prevMousePosition.second - mousePos.second;

    prevMousePosition = mousePos;

    yaw += xDelta * MouseSensitivity;
    pitch += yDelta * MouseSensitivity;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    node->setRotation(glm::vec3(pitch, yaw, 0.0f));
}
