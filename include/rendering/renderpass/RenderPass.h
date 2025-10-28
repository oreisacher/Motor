//
// Created by Orik on 23/10/2025.
//

#ifndef MOTOR_RENDERPASS_H
#define MOTOR_RENDERPASS_H

#include "../RenderPassResourceRegistry.h"
#include <rendering/GPUResourceManager.h>

namespace Motor {
    struct FrameRenderData;

    class RenderPass {
    public:
        virtual void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) = 0;
        virtual ~RenderPass() = default;
    };
}

#endif //MOTOR_RENDERPASS_H