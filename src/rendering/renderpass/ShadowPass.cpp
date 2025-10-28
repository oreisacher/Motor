//
// Created by Orik on 23/10/2025.
//

#include "rendering/renderpass/ShadowPass.h"

#include "assets/AssetManager.h"
#include "assets/ModelInstance.h"
#include "glad/gl.h"
#include "rendering/OpenGLRenderer.h"
#include "utils/ShaderUniformHelper.h"

using namespace Motor;

ShadowPass::ShadowPass(int maxLights, int resolution) : resolution(resolution), maxLights(maxLights) {
    // Load depth shadow shader
    depth = AssetManager::getInstance()->load<Shader>("motor://shaders/depth.shader");

    // Generate framebuffer and texture for each light source
    shadowMapsFBO.resize(maxLights);
    shadowMapTextures.resize(maxLights);

    for (int i = 0; i < maxLights; i++) {
        // Framebuffer
        glGenFramebuffers(1, &shadowMapsFBO[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapsFBO[i]);

        // Texture
        glGenTextures(1, &shadowMapTextures[i]);
        glBindTexture(GL_TEXTURE_2D, shadowMapTextures[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTextures[i], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

ShadowPass::~ShadowPass() {
    for (auto &texture : shadowMapTextures)
        glDeleteTextures(1, &texture);

    for (auto &fbo : shadowMapsFBO)
        glDeleteFramebuffers(1, &fbo);
}

void ShadowPass::execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) {
    glViewport(0, 0, resolution, resolution);

    GPUShader* gpuShader = resourceManager.loadShader(depth);
    glUseProgram(gpuShader->id);

    registry.set("shadowMapResolution", resolution);

    size_t lightCount = std::min(frameData.lights.size(), maxLights);
    for (size_t i = 0; i < lightCount; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapsFBO[i]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &[key, command] : frameData.batchedCommands) {
            GPUMesh* gpuMesh = resourceManager.loadMesh(command.mesh);

            // --- Set Standard Uniforms ---
            setUniformMat4(gpuShader->id, "view", glm::inverse(frameData.lights[i]->getGlobalTransform()));
            setUniformMat4(gpuShader->id, "projection", frameData.lights[i]->getProjectionMatrix());

            // --- Set Additional Uniforms ---
            setUniformBool(gpuShader->id, "instanced", true);

            // --- Set Skinning Uniforms ---
            auto it = frameData.meshModelInstanceLookup.find(command.mesh);
            if (it != frameData.meshModelInstanceLookup.end()) {
                setUniformBool(gpuShader->id, "skinned", it->second->getModel()->isSkinned());

                for (int boneIndex = 0 ; boneIndex < it->second->getModel()->getBoneTransforms().size() ; boneIndex++) {
                    setUniformMat4(gpuShader->id, "u_jointMat[" + std::to_string(boneIndex) + "]", it->second->boneTransforms[boneIndex]);
                }
            }

            resourceManager.updateInstanceBuffer(gpuMesh, command.modelTransforms);

            // --- Draw Call ---
            glBindVertexArray(gpuMesh->vao);
            glDrawElementsInstanced(GL_TRIANGLES, command.mesh->getIndices().size(), GL_UNSIGNED_INT, 0, command.modelTransforms.size());
        }

        // Register shadow map
        registry.set(std::format("shadowMap{}", i), shadowMapTextures[i]);
    }

    glBindVertexArray(0);
}
