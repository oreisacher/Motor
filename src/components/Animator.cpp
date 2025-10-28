//
// Created by Orik on 20/10/2025.
//

#include "components/Animator.h"

#include <format>
#include <core/ModelNode.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace Motor;

void Animator::init() {
    if (dynamic_cast<ModelNode*>(node) == nullptr) {
        LOG_ERROR("Animator can only be attached to a ModelNode")
        return;
    }

    model = dynamic_cast<ModelNode*>(node)->getModelInstance();
}

void Animator::update(float deltaTime) {
    if (!currentAnimation || currentState == PAUSED)
        return;

    currentTime += deltaTime;
    currentTick += deltaTime * currentAnimation->getTicksPerSecond();

    // End of animation reached
    if (currentTick >= currentAnimation->getDuration()) {
        if (shouldLoop) {
            currentTick -= currentAnimation->getDuration();
            currentTime = currentTick * currentAnimation->getTicksPerSecond();
        } else {
            currentTime = 0.0f;
            currentTick = 0;
            currentState = PAUSED;
        }
    }

    calculateAnimationTransforms(model->getModel()->getRoot(), glm::mat4(1.0f));
}

std::vector<std::string> Animator::getAnimationNames() const {
    auto modelNode = dynamic_cast<ModelNode*>(node);
    if (!modelNode) {
        LOG_ERROR("Animator can only be attached to a ModelNode")
        return {};
    };

    auto model = modelNode->getModelInstance();
    if (!model) {
        LOG_ERROR("ModelNode has no model attached")
        return {};
    }

    return model->getModel()->getAnimationNames();
}

void Animator::setAnimation(Animation *animation) {
    currentAnimation = animation;
}

void Animator::setAnimation(const std::string &name) {
    auto modelNode = dynamic_cast<ModelNode*>(node);
    if (!modelNode) {
        LOG_ERROR("Animator can only be attached to a ModelNode")
        return;
    };

    auto model = modelNode->getModelInstance();
    if (!model) {
        LOG_ERROR("ModelNode has no model attached")
        return;
    }

    auto animation = model->getModel()->getAnimation(name);
    if (!animation) {
        LOG_ERROR(std::format("Animation '{}' could not be found", name));
        return;
    }

    currentAnimation = animation;
}

void Animator::setState(STATE state) { currentState = state; }
void Animator::enableLoop(bool enable) { shouldLoop = enable; }

void Animator::reset() {
    currentTime = 0.0f;
    currentTick = 0;
}

void Animator::calculateAnimationTransforms(const Model::Node *node, const glm::mat4 &parentTransform) {
    glm::mat4 localTransform = hasAnimation(node->name) ? getAnimationMatrix(node->name) : node->localTransform;

    model->instanceData[node->name].animationTransform = localTransform;
    model->instanceData[node->name].globalTransform = parentTransform * localTransform;

    glm::mat4 globalTransform = parentTransform * localTransform;

    int boneIndex = model->getModel()->getBone(node->name);
    if (boneIndex >= 0 && boneIndex < model->boneTransforms.size()) {
        model->boneTransforms[boneIndex] = globalTransform * model->getModel()->getBoneTransform(boneIndex);
    }

    for (Model::Node *child : node->children) {
        calculateAnimationTransforms(child, globalTransform);
    }
}

bool Animator::hasAnimation(const std::string &name) const {
    return
        !currentAnimation->getPositionKeyframes(name).empty() ||
        !currentAnimation->getRotationKeyframes(name).empty() ||
        !currentAnimation->getScaleKeyframes(name).empty();
}

glm::mat4 Animator::getAnimationMatrix(const std::string &node) const {
    int currentIndex = 0;
    float progress = 0.0f;

    glm::mat4 animationTransform(1.0f);

    // Translation
    if (!currentAnimation->getPositionKeyframes(node).empty()) {
        auto& keyframes = currentAnimation->getPositionKeyframes(node);
        calculateFrameData(keyframes, currentIndex, progress);
        glm::vec3 interpolated = glm::mix(
            keyframes[currentIndex].data,
            keyframes[(currentIndex + 1) % keyframes.size()].data,
            progress
        );
        animationTransform = glm::translate(animationTransform, interpolated);
    }

    // Rotation
    if (!currentAnimation->getRotationKeyframes(node).empty()) {
        auto& keyframes = currentAnimation->getRotationKeyframes(node);
        calculateFrameData(keyframes, currentIndex, progress);
        animationTransform *= glm::toMat4(
            glm::slerp(
                keyframes[currentIndex].data,
                keyframes[(currentIndex + 1) % keyframes.size()].data,
                progress
            )
        );
    }

    // Scaling
    if (!currentAnimation->getScaleKeyframes(node).empty()) {
        auto& keyframes = currentAnimation->getScaleKeyframes(node);
        calculateFrameData(keyframes, currentIndex, progress);
        glm::vec3 interpolated = glm::mix(keyframes[currentIndex].data,
                                          keyframes[(currentIndex + 1) % keyframes.size()].data,
                                          progress);
        animationTransform = glm::scale(animationTransform, interpolated);
    }

    return animationTransform;
}

// Takes a list of keyframes and calculates current frame as well as the percentage to next frame for interpolation
template<typename T>
void Animator::calculateFrameData(const std::vector<Keyframe<T>>& keyframes, int &currentFrame, float &progress) const {
    if (keyframes.empty() || keyframes.size() == 1) {
        currentFrame = 0;
        progress = 0.0f;
        return;
    }

    for (int i = 0 ; i < keyframes.size() - 1; i++) {
        if (currentTick < keyframes[i + 1].time) {
            currentFrame = i;
            break;
        }
    }

    if (currentFrame >= keyframes.size() - 1) {
        currentFrame = keyframes.size() - 1;
    }

    int nextFrame = currentFrame + 1;
    if (nextFrame >= keyframes.size()) {
        nextFrame = 0;
    }

    float durationBetweenFrames = keyframes[nextFrame].time - keyframes[currentFrame].time;
    if (nextFrame == 0) {
        durationBetweenFrames += currentAnimation->getDuration();
    }

    progress = durationBetweenFrames > 0.0f
        ? (currentTick - keyframes[currentFrame].time) / durationBetweenFrames : 0.0f;
    progress = glm::clamp(progress, 0.0f, 1.0f);
}