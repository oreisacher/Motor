//
// Created by Orik on 25/10/2025.
//

#include "rendering/renderpass/CompositionPass.h"

#include "assets/AssetManager.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "rendering/WindowManager.h"
#include "utils/ShaderUniformHelper.h"

using namespace Motor;

CompositionPass::CompositionPass() {
    fullscreen = AssetManager::getInstance()->load<Shader>("motor://shaders/fullscreen.shader");

    glGenVertexArrays(1, &vao);
}

void CompositionPass::execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_FRAMEBUFFER_SRGB);

    glDisable(GL_DEPTH_TEST);

    auto activeWindow = WindowManager::GetActiveWindow();
    glViewport(0, 0, activeWindow->getWidth(), activeWindow->getHeight());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GPUShader* gpuShader = resourceManager.loadShader(fullscreen);
    glUseProgram(gpuShader->id);

    // Scene Color
    GLuint sceneColor = registry.get("sceneColorTex");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneColor);
    setUniformInt(gpuShader->id, "renderTexture", 0);

    // Bloom
    GLuint bloomTex = registry.get("blurredThresholdTexture");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloomTex);
    setUniformInt(gpuShader->id, "bloomTexture", 1);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glEnable(GL_DEPTH_TEST);
}

void CompositionPass::updateSize(int width, int height) { }
