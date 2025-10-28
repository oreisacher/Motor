//
// Created by Orik on 24/10/2025.
//

#include "rendering/renderpass/SkyboxPass.h"

#include "assets/AssetManager.h"
#include "glad/gl.h"
#include "rendering/OpenGLRenderer.h"
#include "rendering/WindowManager.h"
#include "utils/ShaderUniformHelper.h"

using namespace Motor;

SkyboxPass::SkyboxPass() {
    skybox = AssetManager::getInstance()->load<Material>("motor://materials/skybox.material");
    skyboxMesh = AssetManager::getInstance()->load<Model>("motor://models/skybox")->getMesh(0);
}

void SkyboxPass::execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) {
    glBindFramebuffer(GL_FRAMEBUFFER, registry.get("sceneFBO"));

    auto activeWindow = WindowManager::GetActiveWindow();
    glViewport(0, 0, activeWindow->getWidth(), activeWindow->getHeight());

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GPUShader* gpuShader = resourceManager.loadShader(skybox->getShader());
    glUseProgram(gpuShader->id);

    // --- Set Standard Uniforms ---
    setUniformMat4(gpuShader->id, "view", glm::inverse(frameData.cameras[frameData.activeCamera]->getGlobalTransform()));
    setUniformMat4(gpuShader->id, "projection", frameData.cameras[frameData.activeCamera]->getProjectionMatrix());

    // --- Set Skybox Texture ---
    setUniformVec3(gpuShader->id, "color", skybox->getColor());
    setUniformBool(gpuShader->id, "hasSkybox", skybox->getTexture("skybox") != nullptr);

    if (skybox->getTexture("skybox")) {
        GPUTexture* gpuTexture = resourceManager.loadTexture(skybox->getTexture("skybox"));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gpuTexture->id);
    }

    // --- Draw Skybox ---
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    GPUMesh* gpuMesh = resourceManager.loadMesh(skyboxMesh);
    glBindVertexArray(gpuMesh->vao);
    glDrawElements(GL_TRIANGLES, skyboxMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
