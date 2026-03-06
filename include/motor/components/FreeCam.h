//
// Created by Orik on 20/10/2025.
//

#ifndef MOTOR_FREECAM_H
#define MOTOR_FREECAM_H

#include <utility>

#include "Component.h"

namespace Motor {
    class FreeCam : public Component {
    public:
        void init() override;
        void update(float deltaTime) override;

        float MovementSpeed = 2.5f;
        float MouseSensitivity = 0.1f;

    private:
        float yaw = 0.0f;
        float pitch = 0.0f;

        std::pair<int ,int> prevMousePosition;
    };
}

#endif //MOTOR_FREECAM_H