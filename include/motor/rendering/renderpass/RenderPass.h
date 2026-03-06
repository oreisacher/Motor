//
// Created by Orik on 23/10/2025.
//

#ifndef MOTOR_RENDERPASS_H
#define MOTOR_RENDERPASS_H

#include <motor/rendering/RenderPassResourceRegistry.h>
#include <motor/rendering/GPUResourceManager.h>

namespace Motor {
    struct FrameRenderData;

    class RenderPass {
    public:
        virtual ~RenderPass() = default;

        virtual void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) = 0;
        virtual void updateSize(int width, int height) = 0;
    };
}

#endif //MOTOR_RENDERPASS_H