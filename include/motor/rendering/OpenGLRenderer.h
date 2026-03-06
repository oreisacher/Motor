//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_OPENGLRENDERER_H
#define MOTOR_OPENGLRENDERER_H

#include <motor/core/CameraNode.h>
#include <motor/rendering/IRenderer.h>
#include <motor/rendering/GPUResourceManager.h>
#include <motor/rendering/RenderCommand.h>
#include <motor/rendering/renderpass/RenderPass.h>
#include <motor/core/LightNode.h>

#include "pipeline/RenderPipeline.h"

namespace Motor {
    struct BatchKey {
        const Mesh* mesh;
        const Material* material;

        bool operator==(const BatchKey& other) const {
            return mesh == other.mesh && material == other.material;
        }
    };

    struct BatchKeyHasher {
        std::size_t operator()(const BatchKey& k) const {
            return std::hash<const void*>()(k.mesh) ^ (std::hash<const void*>()(k.material) << 1);
        }
    };

    struct FrameRenderData {
        std::unordered_map<const Mesh*, const ModelInstance*> meshModelInstanceLookup = {};

        std::unordered_map<BatchKey, RenderCommand, BatchKeyHasher> batchedCommands = {};
        std::vector<LightNode*> lights = {};

        std::vector<CameraNode*> cameras = {};
        int activeCamera = -1;
    };

    class OpenGLRenderer : public IRenderer {
    public:
        bool init() override;
        void shutdown() override;

        void setPipeline(RenderPipeline* pl);

        void render(Scene* scene) override;

        void updateWindowSize(int width, int height);

    private:
        void processScene(SceneNode* node, FrameRenderData& frameData);
        void findActiveCamera(FrameRenderData& frameData);
        void recreateFramebuffer(int width, int height);

        GPUResourceManager resourceManager;
        RenderPipeline* pipeline = nullptr;

        RenderPassResourceRegistry registry;

        unsigned int sceneFBO, sceneColorTex, sceneDepthRBO;
    };
}

#endif //MOTOR_OPENGLRENDERER_H