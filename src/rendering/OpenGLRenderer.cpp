//
// Created by Orik on 16/10/2025.
//

#define GLAD_GL_IMPLEMENTATION

#include <motor/rendering/OpenGLRenderer.h>
#include <motor/rendering/RenderCommand.h>
#include <motor/utils/Logger.h>
#include <motor/core/ModelNode.h>

#include <format>
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <motor/assets/AssetManager.h>
#include <motor/rendering/RenderPassResourceRegistry.h>
#include <motor/rendering/WindowManager.h>
#include <motor/rendering/renderpass/ShaderPass.h>
#include <motor/rendering/renderpass/ForwardRenderPass.h>
#include <motor/rendering/renderpass/CompositionPass.h>
#include <motor/rendering/renderpass/ShadowPass.h>
#include <motor/rendering/renderpass/SkyboxPass.h>

using namespace Motor;

bool OpenGLRenderer::init() {
    if (!gladLoadGL(glfwGetProcAddress)) {
        LOG_FATAL("Failed to initialize GLAD");
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void OpenGLRenderer::shutdown() { }

void OpenGLRenderer::setPipeline(RenderPipeline* pl) { pipeline = pl; }

void OpenGLRenderer::render(Scene* scene) {
    if (!pipeline) return;

    FrameRenderData frameData;

    if (scene) {
        processScene(scene->getRoot(), frameData);
        findActiveCamera(frameData);
    }

    for (auto& pass : pipeline->getPasses()) {
        pass->execute(frameData, registry, resourceManager);
    }
}

void OpenGLRenderer::updateWindowSize(const int width, const int height) {
    if (!pipeline) return;

    recreateFramebuffer(width, height);

    for (auto& pass : pipeline->getPasses())
        pass->updateSize(width, height);
}

void OpenGLRenderer::processScene(SceneNode *node, FrameRenderData &frameData) {
    // Check if node is model > Create new RenderCommands
    if (node->getType() == "ModelNode") {
        ModelNode* modelNode = static_cast<ModelNode*>(node);
        auto modelInstance = modelNode->getModelInstance();

        if (modelInstance) {
            for (auto& meshNode : modelInstance->getModel()->getMeshNodes()) {
                for (auto& meshIndex : meshNode->meshes) {
                    auto mesh = modelInstance->getModel()->getMesh(meshIndex);
                    frameData.meshModelInstanceLookup[mesh] = modelInstance;

                    auto material = modelInstance->getMaterialForMesh(mesh->getName());
                    BatchKey key{ mesh, material };

                    glm::mat4 worldTransform =
                        modelNode->getGlobalTransform() *
                        modelInstance->instanceData[meshNode->name].globalTransform;

                    auto it = frameData.batchedCommands.find(key);
                    if (it == frameData.batchedCommands.end()) {
                        // Create new batch entry
                        frameData.batchedCommands.emplace(
                            key,
                            RenderCommand{
                                mesh,
                                material,
                                { worldTransform } // initialize vector with first transform
                            }
                        );
                    } else {
                        // Add to existing batch
                        it->second.modelTransforms.push_back(worldTransform);
                    }
                }
            }
        }
    }

    if (node->getType() == "CameraNode") {
        frameData.cameras.push_back(static_cast<CameraNode*>(node));
    }

    if (node->getType() == "LightNode") {
        frameData.lights.push_back(static_cast<LightNode*>(node));
    }

    for (int i = 0 ; i < node->getChildCount(); i++) {
        processScene(node->getChild(i), frameData);
    }
}

void OpenGLRenderer::findActiveCamera(FrameRenderData &frameData) {
    for (int i = 0 ; i < frameData.cameras.size(); i++) {
        if (frameData.cameras[i]->isActive()) {
            frameData.activeCamera = i;
            return;
        }
    }
}

void OpenGLRenderer::recreateFramebuffer(const int width, const int height) {
    if (sceneColorTex) glDeleteTextures(1, &sceneColorTex);
    if (sceneDepthRBO) glDeleteRenderbuffers(1, &sceneDepthRBO);
    if (sceneFBO) glDeleteFramebuffers(1, &sceneFBO);

    // Global Scene Framebuffer
    glGenFramebuffers(1, &sceneFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

    glGenTextures(1, &sceneColorTex);
    glBindTexture(GL_TEXTURE_2D, sceneColorTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColorTex, 0);

    glGenRenderbuffers(1, &sceneDepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, sceneDepthRBO);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sceneDepthRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Update registry
    registry.set("sceneFBO", sceneFBO);
    registry.set("sceneColorTex", sceneColorTex);
    registry.set("sceneDepthRBO", sceneDepthRBO);
}
