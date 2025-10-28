//
// Created by Orik on 12/10/2025.
//

#ifndef MOTOR_CAMERANODE_H
#define MOTOR_CAMERANODE_H

#include "SceneNode.h"

namespace Motor {
    class CameraNode : public SceneNode {
    public:
        CameraNode();
        CameraNode(const std::string &name, float param, float aspect, float near, float far, int orthographic = false);

        float getFOV() const;
        void setFOV(float fov);

        float getAspect() const;
        void setAspect(float aspect);

        float getNear() const;
        void setNear(float near);

        float getFar() const;
        void setFar(float far);

        bool isOrthographic() const;
        void setOrthographic(bool ortho);

        float getOrthographicSize() const;
        void setOrthographicSize(float size);

        bool isActive() const;
        void setActive(bool active);

        glm::mat4 getProjectionMatrix() const;

        std::string toString() const override;
        std::string getType() const override;

    private:
        float fov = 90.0f;
        float aspect = 16.0f / 9.0f;

        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        bool orthographic = false;
        float orthoSize = 10.0f;

        bool active = false;

        glm::mat4 projectionMatrix;

        static inline int count = 0;

        void updateProjectionMatrix();

    };
}

#endif //MOTOR_CAMERANODE_H