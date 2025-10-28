//
// Created by Orik on 18/09/2025.
//

#ifndef MOTOR_SCRIPT_H
#define MOTOR_SCRIPT_H

namespace Motor {
    class SceneNode;

    class Component {
    public:
        virtual ~Component() = default;

        virtual void init() = 0;
        virtual void update(float deltaTime) = 0;

        bool isEnabled();
        void setEnabled(bool enabled);

    protected:
        SceneNode* node = nullptr;

    private:
        bool enabled = true;

        friend class SceneNode;
    };
}

#endif //MOTOR_SCRIPT_H