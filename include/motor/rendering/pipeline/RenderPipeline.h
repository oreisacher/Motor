//
// Created by ruut on 3/6/26.
//

#ifndef MOTOR_RENDERPIPELINE_H
#define MOTOR_RENDERPIPELINE_H

#include "motor/rendering/renderpass/RenderPass.h"
#include <memory>

namespace Motor {
    class RenderPipeline {
    public:
        void addPass(std::unique_ptr<RenderPass> pass);
        const std::vector<std::unique_ptr<RenderPass>>& getPasses() const;

    private:
        std::vector<std::unique_ptr<RenderPass>> passes;
    };
}

#endif //MOTOR_RENDERPIPELINE_H