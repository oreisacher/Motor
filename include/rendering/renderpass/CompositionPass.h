//
// Created by Orik on 25/10/2025.
//

#ifndef MOTOR_FULLSCREENPASS_H
#define MOTOR_FULLSCREENPASS_H

#include "RenderPass.h"

namespace Motor {
    class CompositionPass : public RenderPass {
    public:
        CompositionPass();

        void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) override;

    private:
        Shader* fullscreen;

        unsigned int vao;
    };
}

#endif //MOTOR_FULLSCREENPASS_H