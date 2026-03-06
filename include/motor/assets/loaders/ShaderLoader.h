//
// Created by Orik on 21/09/2025.
//

#ifndef MOTOR_SHADERLOADER_H
#define MOTOR_SHADERLOADER_H

#include <motor/assets/Shader.h>
#include "AssetLoader.h"

namespace Motor {
    class ShaderLoader : public AssetLoader {
    public:
        std::unique_ptr<Asset> load(const std::string &path) override;
        void registerDefaultAssets() override;
    };
}

#endif //MOTOR_SHADERLOADER_H