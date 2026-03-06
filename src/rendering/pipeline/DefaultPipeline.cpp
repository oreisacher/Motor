//
// Created by ruut on 3/6/26.
//

#include "../../../include/motor/rendering/pipeline/DefaultPipeline.h"
#include "motor/assets/AssetManager.h"
#include "motor/rendering/renderpass/CompositionPass.h"
#include "motor/rendering/renderpass/ForwardRenderPass.h"
#include "motor/rendering/renderpass/ShaderPass.h"
#include "motor/rendering/renderpass/ShadowPass.h"
#include "motor/rendering/renderpass/SkyboxPass.h"

using namespace Motor;

std::unique_ptr<RenderPipeline> DefaultPipeline::Create() {
    auto pipeline = std::make_unique<RenderPipeline>();

    // Setup blur shader
    auto blurShader = AssetManager::getInstance()->load<Shader>("motor://shaders/blur.shader");
    auto xBlur = std::make_shared<ShaderInstance>(blurShader);
    xBlur->setUniform("horizontal", false);
    auto yBlur = std::make_shared<ShaderInstance>(blurShader);
    yBlur->setUniform("horizontal", true);

    pipeline->addPass(std::make_unique<ShadowPass>());

    pipeline->addPass(std::make_unique<SkyboxPass>());

    pipeline->addPass(std::make_unique<ForwardRenderPass>());

    pipeline->addPass(std::make_unique<ShaderPass>(
        "sceneFBO",
        "brightThresholdTexture",
        AssetManager::getInstance()->load<Shader>("motor://shaders/brightThreshold.shader")->getDefaultInstance()));

    pipeline->addPass(std::make_unique<ShaderPass>(
            "brightThresholdTexture",
            "blurredThresholdXTexture",
            xBlur));

    pipeline->addPass(std::make_unique<ShaderPass>(
            "blurredThresholdXTexture",
            "blurredThresholdTexture",
            yBlur));

    pipeline->addPass(std::make_unique<CompositionPass>());

    return pipeline;
}
