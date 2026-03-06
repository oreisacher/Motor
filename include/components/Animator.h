//
// Created by Orik on 20/10/2025.
//

#ifndef MOTOR_ANIMATOR_H
#define MOTOR_ANIMATOR_H

#include <assets/Animation.h>
#include <assets/Model.h>

#include "Component.h"

namespace Motor {
    class Animator : public Component {
    public:
        enum STATE {
            PLAYING,
            PAUSED
        };

        void init() override;
        void update(float deltaTime) override;

        std::vector<std::string> getAnimationNames() const;
        void setAnimation(Animation* animation);
        void setAnimation(const std::string &name);

        void setState(STATE state);
        void enableLoop(bool enable);

        void reset();

    private:
        Animation* currentAnimation = nullptr;
        ModelInstance* model = nullptr;

        double currentTime = 0.0;
        std::uint64_t currentTick = 0;
        STATE currentState = PAUSED;
        bool shouldLoop = true;

        void calculateAnimationTransforms(const Model::Node* node, const glm::mat4 &parentTransform);
        bool hasAnimation(const std::string &name) const;
        glm::mat4 getAnimationMatrix(const std::string &node) const;

        template<typename T>
        void calculateFrameData(const std::vector<Keyframe<T>>& keyframes, int &currentFrame, float &progress) const;
    };
}


#endif //MOTOR_ANIMATOR_H