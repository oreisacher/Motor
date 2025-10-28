//
// Created by Orik on 20/10/2025.
//

#ifndef MOTOR_ANIMATION_H
#define MOTOR_ANIMATION_H

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

namespace Motor {
    enum KEYFRAME_TYPE {
        POSITION,
        ROTATION,
        SCALE
    };

    template<typename T>
    struct Keyframe {
        double time;
        T data;
    };

    using PositionKeyframe = Keyframe<glm::vec3>;
    using RotationKeyframe = Keyframe<glm::quat>;
    using ScaleKeyframe    = Keyframe<glm::vec3>;

    class Animation {
    public:
        Animation(std::string name);

        float getDuration() const;
        float getTicksPerSecond() const;

        const std::map<std::string, std::vector<PositionKeyframe>>& getPositionKeyframes();
        const std::vector<PositionKeyframe>& getPositionKeyframes(const std::string &channel);

        const std::map<std::string, std::vector<RotationKeyframe>>& getRotationKeyframes();
        const std::vector<RotationKeyframe>& getRotationKeyframes(const std::string &channel);

        const std::map<std::string, std::vector<ScaleKeyframe>>& getScaleKeyframes();
        const std::vector<ScaleKeyframe>& getScaleKeyframes(const std::string &channel);

        template <typename T>
        void addKeyframe(const std::string &node, KEYFRAME_TYPE type, double time, T data) {
            if constexpr (std::is_same_v<T, glm::vec3>) {
                if (type == POSITION)
                    positionKeyframes[node].push_back({ time, data });
                else if (type == SCALE)
                    scaleKeyframes[node].push_back({ time, data });
            } else if constexpr (std::is_same_v<T, glm::quat>) {
                rotationKeyframes[node].push_back({ time, data });
            }
        }

    private:
        std::string name;

        float duration = 0.0f;
        float ticksPerSecond = 0.0f;

        // Animation keyframe data
        std::map<std::string, std::vector<PositionKeyframe>> positionKeyframes;
        std::map<std::string, std::vector<RotationKeyframe>> rotationKeyframes;
        std::map<std::string, std::vector<ScaleKeyframe>> scaleKeyframes;

        friend class ModelLoader;
    };
}

#endif //MOTOR_ANIMATION_H