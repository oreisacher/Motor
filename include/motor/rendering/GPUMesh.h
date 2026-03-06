//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_GPUMESH_H
#define MOTOR_GPUMESH_H

#include "GPUResource.h"
#include <cstddef>

namespace Motor {
    class GPUMesh : public GPUResource {
    public:
        GPUMesh(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int instanceVBO);

        unsigned int vao, vbo, ebo;
        unsigned int instanceVBO = 0;

        size_t instanceDataSize = 0;

        void release() override;
    };
}

#endif //MOTOR_GPUMESH_H