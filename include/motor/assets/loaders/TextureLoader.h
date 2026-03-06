//
// Created by Orik on 21/09/2025.
//

#ifndef MOTOR_TEXTURELOADER_H
#define MOTOR_TEXTURELOADER_H

#include <memory>

#include "../Texture.h"
#include "AssetLoader.h"

namespace Motor {
    class TextureLoader : public AssetLoader {
    public:
        TextureLoader();
        std::unique_ptr<Asset> load(const std::string &path) override;
        void registerDefaultAssets() override;
    };
}

#endif //MOTOR_TEXTURELOADER_H