//
// Created by ruut on 3/6/26.
//

#ifndef MOTOR_FULLSCREENPASS_H
#define MOTOR_FULLSCREENPASS_H

#include "motor/rendering/renderpass/RenderPass.h"

namespace Motor {
    struct FrameRenderData;

    class FullscreenPass : public Motor::RenderPass {
    public:
        FullscreenPass(Shader* shr);

        void execute(const FrameRenderData& frameData, RenderPassResourceRegistry& registry, GPUResourceManager& resourceManager) override;
        void updateSize(int width, int height) override;

    private:
        Shader* shader;
        unsigned int vao;
    };
}

#endif //MOTOR_FULLSCREENPASS_H