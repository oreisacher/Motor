//
// Created by Orik on 23/10/2025.
//

#ifndef MOTOR_FORWARDRENDERPASS_H
#define MOTOR_FORWARDRENDERPASS_H

#include "RenderPass.h"

namespace Motor {
    class ForwardRenderPass : public RenderPass {
    public:
        ForwardRenderPass();

        void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) override;
        ~ForwardRenderPass() override = default;

    private:
        Material* skybox;
    };
}

#endif //MOTOR_FORWARDRENDERPASS_H