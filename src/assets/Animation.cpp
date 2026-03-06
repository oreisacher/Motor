//
// Created by Orik on 20/10/2025.
//

#include <motor/assets/Animation.h>
#include <utility>

using namespace Motor;

Animation::Animation(std::string name) : name(std::move(name)) { }

float Animation::getDuration() const { return duration; }
float Animation::getTicksPerSecond() const { return ticksPerSecond; }

const std::map<std::string, std::vector<PositionKeyframe>> & Animation::getPositionKeyframes() { return positionKeyframes; }

const std::vector<PositionKeyframe>& Animation::getPositionKeyframes(const std::string &channel) {
    static std::vector<PositionKeyframe> empty;
    const auto it = positionKeyframes.find(channel);
    return (it != positionKeyframes.end()) ? it->second : empty;
}

const std::map<std::string, std::vector<RotationKeyframe>> & Animation::getRotationKeyframes() { return rotationKeyframes; }

const std::vector<RotationKeyframe>& Animation::getRotationKeyframes(const std::string &channel) {
    static std::vector<RotationKeyframe> empty;
    const auto it = rotationKeyframes.find(channel);
    return (it != rotationKeyframes.end()) ? it->second : empty;
}

const std::map<std::string, std::vector<ScaleKeyframe>> & Animation::getScaleKeyframes() { return scaleKeyframes; }

const std::vector<ScaleKeyframe>& Animation::getScaleKeyframes(const std::string &channel) {
    static std::vector<ScaleKeyframe> empty;
    const auto it = scaleKeyframes.find(channel);
    return (it != scaleKeyframes.end()) ? it->second : empty;
}
