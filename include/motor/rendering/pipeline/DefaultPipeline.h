//
// Created by ruut on 3/6/26.
//

#ifndef MOTOR_DEFAULTPIPELINE_H
#define MOTOR_DEFAULTPIPELINE_H

#include "RenderPipeline.h"
#include <memory>

namespace Motor {
    class DefaultPipeline {
    public:
        static std::unique_ptr<RenderPipeline> Create();
    };
}

#endif //MOTOR_DEFAULTPIPELINE_H