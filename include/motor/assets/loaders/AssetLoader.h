//
// Created by Orik on 21/09/2025.
//

#ifndef MOTOR_ASSETLOADER_H
#define MOTOR_ASSETLOADER_H

#include <motor/utils/Logger.h>
#include <motor/assets/Asset.h>
// #include <motor/assets/AssetManager.h>

namespace Motor {
    class AssetManager;

    class AssetLoader {
    public:
        virtual ~AssetLoader() = default;
        virtual std::unique_ptr<Asset> load(const std::string& path) = 0;
        virtual void registerDefaultAssets() = 0;
    };
}

#endif //MOTOR_ASSETLOADER_H