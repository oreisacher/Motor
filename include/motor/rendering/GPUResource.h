//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_GPURESOURCE_H
#define MOTOR_GPURESOURCE_H

namespace Motor {
    class GPUResource {
    public:
        virtual ~GPUResource() = default;
        virtual void release() = 0;
    };
}

#endif //MOTOR_GPURESOURCE_H