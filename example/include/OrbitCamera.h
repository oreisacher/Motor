//
// Created by Orik on 28/10/2025.
//

#ifndef PROJECT_ORBITCAMERA_H
#define PROJECT_ORBITCAMERA_H

#include <utility>
#include <motor/components/Component.h>

class OrbitCamera : public Motor::Component {
public:
    void init() override;
    void update(float deltaTime) override;

    float MovementSpeed = 2.5f;
    float MouseSensitivity = 5.0f;

    Motor::SceneNode* target;

private:
    float yaw = 0.0f;
    float pitch = 0.0f;
    float radius = 2.0f;

    std::pair<int ,int> prevMousePosition;
};

#endif //PROJECT_ORBITCAMERA_H