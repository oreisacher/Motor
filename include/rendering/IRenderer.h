//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_IRENDERER_H
#define MOTOR_IRENDERER_H

#include <core/Scene.h>

namespace Motor {
    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual bool init() = 0;
        virtual void shutdown() = 0;

        virtual void render(Scene* scene) = 0;
    };
}

#endif //MOTOR_IRENDERER_H