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
        ~SkyboxPass() override = default;

        void execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) override;
        void updateSize(int width, int height) override;

    private:
        Material* skybox;
        Mesh* skyboxMesh;

        int width = 0, height = 0;
    };
}


#endif //MOTOR_SKYBOXPASS_H