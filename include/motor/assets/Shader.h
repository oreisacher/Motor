//
// Created by Orik on 21/09/2025.
//

#ifndef MOTOR_SHADER_H
#define MOTOR_SHADER_H

#include <motor/core/ShaderInstance.h>

#include <map>
#include <memory>

#include "Asset.h"
#include "ShaderSource.h"

namespace Motor {
    class Shader : public Asset {
    public:
        ShaderSource* getSource(const ShaderSource::Type &type);
        std::vector<ShaderSource*> getSources() const;

        void addSource(ShaderSource* shaderSource);
        void removeSource(const ShaderSource::Type &type);

        std::shared_ptr<ShaderInstance> getDefaultInstance();

        const char* getType() const override;

    private:
        std::map<ShaderSource::Type, ShaderSource*> data;
        std::shared_ptr<ShaderInstance> defaultInstance = nullptr;

        friend class ShaderLoader;
    };
}

#endif //MOTOR_SHADER_H