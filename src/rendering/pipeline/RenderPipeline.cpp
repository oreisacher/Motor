//
// Created by ruut on 3/6/26.
//

#include "motor/rendering/pipeline/RenderPipeline.h"

void RenderPipeline::addPass(std::unique_ptr<Motor::RenderPass> pass) {
    passes.push_back(std::move(pass));
}

const std::vector<std::unique_ptr<Motor::RenderPass>>& RenderPipeline::getPasses() const {
    return passes;
}
