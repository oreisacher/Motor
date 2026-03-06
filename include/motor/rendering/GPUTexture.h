//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_GPUTEXTURE_H
#define MOTOR_GPUTEXTURE_H

#include "GPUResource.h"

namespace Motor {
    class GPUTexture : public GPUResource {
    public:
        GPUTexture(unsigned int id);

        unsigned int id;

        void release() override;
    };
}

#endif //MOTOR_GPUTEXTURE_H