//
// Created by Orik on 22/09/2025.
//

#ifndef MOTOR_SHADERSOURCE_H
#define MOTOR_SHADERSOURCE_H

#include "Asset.h"

namespace Motor {
    class ShaderSource : public Asset {
    public:
        enum Type {
            VERTEX,
            FRAGMENT,
            TESSELLATION_CONTROL,
            TESSELLATION_EVAL,
            GEOMETRY,
            UNKNOWN
        };

        const std::string& getData();
        const Type& getShaderType() const;

        const char* getType() const override;

    private:
        std::string data;
        Type type = UNKNOWN;

        friend class ShaderSourceLoader;
    };
}

#endif //MOTOR_SHADERSOURCE_H