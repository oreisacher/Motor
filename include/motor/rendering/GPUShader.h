//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_GPUSHADER_H
#define MOTOR_GPUSHADER_H

#include "GPUResource.h"

namespace Motor {
    class GPUShader : public GPUResource {
    public:
        GPUShader(unsigned int id);

        unsigned int id;

        void release() override;
    };
}

#endif //MOTOR_GPUSHADER_H