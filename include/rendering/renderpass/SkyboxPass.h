//
// Created by Orik on 24/10/2025.
//

#ifndef MOTOR_SKYBOXPASS_H
#define MOTOR_SKYBOXPASS_H

#include "RenderPass.h"

namespace Motor {
    class SkyboxPass : public RenderPass {
    public:
        SkyboxPass();

        void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) override;
        ~SkyboxPass() override = default;

    private:
        Material* skybox;
        Mesh* skyboxMesh;
    };
}


#endif //MOTOR_SKYBOXPASS_H