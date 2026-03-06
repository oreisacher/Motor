//
// Created by ruut on 3/6/26.
//

#include "motor/rendering/WindowManager.h"
#include "motor/rendering/renderpass/FullscreenPass.h"
#include "glad/gl.h"

Motor::FullscreenPass::FullscreenPass(Shader* shr) {
    setShader(shr);

    glGenVertexArrays(1, &vao);
}

void Motor::FullscreenPass::setShader(Shader* shr) {
    shader = shr;
}

void Motor::FullscreenPass::execute(const FrameRenderData& frameData, RenderPassResourceRegistry& registry, GPUResourceManager& resourceManager) {
    if (!shader) return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    auto window = WindowManager::GetActiveWindow();
    glViewport(0, 0, window->getWidth(), window->getHeight());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GPUShader* gpuShader = resourceManager.loadShader(shader);
    glUseProgram(gpuShader->id);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glEnable(GL_DEPTH_TEST);
}

void Motor::FullscreenPass::updateSize(int width, int height) { }
