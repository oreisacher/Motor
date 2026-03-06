//
// Created by Orik on 23/09/2025.
//

#ifndef MOTOR_MATERIALLOADER_H
#define MOTOR_MATERIALLOADER_H

#include <assets/Material.h>
#include "AssetLoader.h"

namespace Motor {
    class MaterialLoader : public AssetLoader {
    public:
        std::unique_ptr<Asset> load(const std::string &path) override;
        void registerDefaultAssets() override;
    };
}

#endif //MOTOR_MATERIALLOADER_H