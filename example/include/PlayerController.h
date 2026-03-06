//
// Created by Orik on 28/10/2025.
//

#ifndef PROJECT_PLAYERCONTROLLER_H
#define PROJECT_PLAYERCONTROLLER_H

#include <motor/components/Animator.h>
#include <motor/components/Component.h>

class PlayerController : public Motor::Component {
public:
    void init() override;
    void update(float deltaTime) override;

    Motor::SceneNode* camera;
    Motor::Animator* animator;
    bool isMoving = false;
};

#endif //PROJECT_PLAYERCONTROLLER_H