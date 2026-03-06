//
// Created by Orik on 22/09/2025.
//

#ifndef MOTOR_SHADERSOURCELOADER_H
#define MOTOR_SHADERSOURCELOADER_H

#include "AssetLoader.h"
#include "../ShaderSource.h"

namespace Motor {
    class ShaderSourceLoader : public AssetLoader {
    public:
        std::unique_ptr<Asset> load(const std::string &path) override;
        void registerDefaultAssets() override;

    private:
        ShaderSource::Type convert(const std::string &type);
    };
}

#endif //MOTOR_SHADERSOURCELOADER_H