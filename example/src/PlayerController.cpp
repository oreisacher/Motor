//
// Created by Orik on 28/10/2025.
//

#include "PlayerController.h"

#include <components/Animator.h>
#include <core/InputManager.h>
#include <core/SceneNode.h>
#include <glm/glm.hpp>
#include <rendering/Window.h>

void PlayerController::init() {
    animator = node->getComponent<Motor::Animator>();
}

void PlayerController::update(float deltaTime) {
    glm::mat4 transform = camera->getGlobalTransform();

    glm::vec3 front = -glm::vec3(transform[2]);
    glm::vec3 right = glm::vec3(transform[0]);

    front.y = 0.0f;
    right.y = 0.0f;

    front = glm::normalize(front);
    right = glm::normalize(right);

    float velocity = 1.8f * deltaTime;
    glm::vec3 deltaPos(0.0f);

    if (Motor::Input.isKeyPressed(Motor::Window::Key::W))
        deltaPos += front;
    if (Motor::Input.isKeyPressed(Motor::Window::Key::S))
        deltaPos -= front;
    if (Motor::Input.isKeyPressed(Motor::Window::Key::A))
        deltaPos -= right;
    if (Motor::Input.isKeyPressed(Motor::Window::Key::D))
        deltaPos += right;

    if (glm::length(deltaPos) > 0.001f)
        deltaPos = glm::normalize(deltaPos) * velocity;

    node->setPosition(node->getPosition() + deltaPos);

    bool currentlyMoving = glm::length(deltaPos) > 0.0f;

    if (currentlyMoving) {
        glm::vec3 lookDir = glm::normalize(glm::vec3(deltaPos.x, 0.0f, deltaPos.z));
        float targetYaw = atan2(lookDir.x, lookDir.z);

        // Current rotation as quaternion
        glm::quat currentRot = node->getRotation();

        // Target rotation quaternion (Y-axis only)
        glm::quat targetRot = glm::angleAxis(targetYaw, glm::vec3(0,1,0));

        // Smoothly interpolate rotation
        float turnSpeed = 10.0f; // bigger = faster turning
        glm::quat smoothRot = glm::slerp(currentRot, targetRot, glm::clamp(turnSpeed * deltaTime, 0.0f, 1.0f));

        node->setRotation(glm::degrees(glm::eulerAngles(smoothRot)));
    }

    /*
     *  Notice the note in main.cpp about animated characters
     *

    if (currentlyMoving && !isMoving) {
        // Movement just started → play animation
        animator->setAnimation("walk");
    } else if (!currentlyMoving && isMoving) {
        // Movement just stopped → pause animation
        animator->setAnimation("idle");
    }
    */

    // Update movement state for next frame
    isMoving = currentlyMoving;
}
