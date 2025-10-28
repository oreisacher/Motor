//
// Created by Orik on 22/10/2025.
//

#ifndef MOTOR_LIGHTNODE_H
#define MOTOR_LIGHTNODE_H

#include "SceneNode.h"

namespace Motor {
    class LightNode : public SceneNode {
    public:
        enum class Type {
            DIRECTIONAL,
            POINT,
            SPOT
        };

        LightNode();
        LightNode(const std::string& name);
        LightNode(const std::string& name, Type type);

        Type getLightType() const;
        void setLightType(Type type);

        glm::vec3 getColor() const;
        void setColor(const glm::vec3 &color);

        float getIntensity() const;
        void setIntensity(float intensity);

        float getLinear() const;
        void setLinear(float linear);

        float getQuadratic() const;
        void setQuadratic(float quadratic);

        float getInnerAngle() const;
        void setInnerAngle(float innerAngle);

        float getOuterAngle() const;
        void setOuterAngle(float outerAngle);

        glm::vec3 getDirection() const;

        glm::mat4 getProjectionMatrix() const;

        std::string toString() const override;
        std::string getType() const override;

    private:
        Type type = Type::POINT;

        glm::vec3 color = glm::vec3(1.0f);
        float intensity = 1.0f;

        // Range for point/spotlights. Does not affect directional lights
        float linear = 0.09f;
        float quadratic = 0.032f;

        float spotInnerAngle = 15.0f;
        float spotOuterAngle = 30.0f;

        glm::mat4 projection = glm::mat4(1.0f);

        void recalculateProjection();
    };
}

#endif //MOTOR_LIGHTNODE_H