//
// Created by Orik on 23/09/2025.
//

#ifndef MOTOR_ASSETMANAGER_H
#define MOTOR_ASSETMANAGER_H

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <utils/Logger.h>

#include "Asset.h"
#include "BenzinLoader.h"
#include "loaders/AssetLoader.h"

namespace Motor {
    class AssetLoader;

    class AssetManager {
    public:
        template<typename T>
        T* load(const std::string& name) {
            // Check if asset is already loaded
            auto it = loadedAssets.find(name);
            if (it != loadedAssets.end()) {
                return dynamic_cast<T*>(it->second.get());
            }

            if (!resourceLoader.isInitialized()) {
                LOG_ERROR("Resource loader not initialized");
                return nullptr;
            }

            // Load asset
            auto loader = assetLoaders.find(typeid(T).name());
            if (loader == assetLoaders.end()) {
                LOG_ERROR("No asset loader for specified type found");
                return nullptr;
            }

            auto asset = loader->second->load(name);

            // Register asset if loaded successfully
            if (asset) {
                loadedAssets[name] = std::move(asset);
            }

            return dynamic_cast<T*>(loadedAssets[name].get());
        }

        template<typename T>
        void registerAsset(const std::string &name, T* asset) {
            if (!asset)
                return;
            loadedAssets[name] = std::unique_ptr<T>(asset);
        }

        bool isLoaded(const std::string &name) const;

        // Set resource file for asset loading
        void setResourceFile(const std::string& path = "resources.benzin");

        // Load raw bytes from resource file
        std::vector<char> readBytes(const std::string& path) const;

        BenzinLoader getLoader();

        // Get current amount of cached assets
        int getCacheCount() const;

        static AssetManager* getInstance();

    private:
        AssetManager() = default;
        static AssetManager* manager;

        BenzinLoader resourceLoader;

        std::unordered_map<std::string, std::unique_ptr<Asset>> loadedAssets;
        std::unordered_map<std::string, std::unique_ptr<AssetLoader>> assetLoaders;

        void init();
    };
}

#endif //MOTOR_ASSETMANAGER_H