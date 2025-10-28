//
// Created by Orik on 25/10/2025.
//

#ifndef MOTOR_BRIGHTPASS_H
#define MOTOR_BRIGHTPASS_H

#include "RenderPass.h"

namespace Motor {
    class ShaderPass : public RenderPass {
    public:
        ShaderPass(const std::string &inTexture, const std::string &outTexture, std::shared_ptr<ShaderInstance> shader);

        void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) override;
    private:
        std::shared_ptr<ShaderInstance> shader;

        std::string inTexture, outTexture;

        unsigned int vao;

        // Framebuffer
        unsigned int fbo;
        unsigned int fboTexture;
    };
}

#endif //MOTOR_BRIGHTPASS_H