//
// Created by Orik on 23/10/2025.
//

#ifndef MOTOR_SHADOWPASS_H
#define MOTOR_SHADOWPASS_H

#include "RenderPass.h"

namespace Motor {
    class ShadowPass : public RenderPass {
    public:
        ShadowPass(int maxLights = 8, int resolution = 4098);
        ~ShadowPass();

        void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) override;

    private:
        int resolution = 1024;
        size_t maxLights = 8;

        Shader* depth = nullptr;

        std::vector<unsigned int> shadowMapsFBO;
        std::vector<unsigned int> shadowMapTextures;
    };
}

#endif //MOTOR_SHADOWPASS_H