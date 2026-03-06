//
// Created by Orik on 22/10/2025.
//

#include <motor/core/LightNode.h>
#include <format>

using namespace Motor;

LightNode::LightNode() = default;
LightNode::LightNode(const std::string &name) : SceneNode(name) {}
LightNode::LightNode(const std::string &name, const Type type) : SceneNode(name) {
    setLightType(type);
}

LightNode::Type LightNode::getLightType() const { return type; }
void LightNode::setLightType(Type type) {
    this->type = type;
    recalculateProjection();
}

glm::vec3 LightNode::getColor() const { return color; }
void LightNode::setColor(const glm::vec3 &color) { this->color = color; }

float LightNode::getIntensity() const { return intensity; }
void LightNode::setIntensity(float intensity) { this->intensity = intensity; }

float LightNode::getLinear() const { return linear; }
void LightNode::setLinear(float linear) { this->linear = linear; }

float LightNode::getQuadratic() const { return quadratic; }
void LightNode::setQuadratic(float quadratic) { this->quadratic = quadratic; }

float LightNode::getInnerAngle() const { return spotInnerAngle; }
void LightNode::setInnerAngle(const float innerAngle) { this->spotInnerAngle = innerAngle; }

float LightNode::getOuterAngle() const { return spotOuterAngle; }
void LightNode::setOuterAngle(const float outerAngle) {
    this->spotOuterAngle = outerAngle;
    recalculateProjection();
}

glm::vec3 LightNode::getDirection() const {
    return glm::normalize(globalRotation * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::mat4 LightNode::getProjectionMatrix() const { return projection;}

std::string LightNode::toString() const {
    return std::format("LightNode ({}) - Type: {}", name, static_cast<int>(type));
}

std::string LightNode::getType() const { return "LightNode"; }

void LightNode::recalculateProjection() {
    switch (type) {
        case Type::DIRECTIONAL: {
            projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);
            break;
        }

        case Type::SPOT: {
            projection = glm::perspective(glm::radians(spotOuterAngle * 2.0f), 1.0f, 0.1f, 50.0f);
            break;
        }

        case Type::POINT: {
            projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 50.0f);
            break;
        }
    }
}
