//
// Created by Orik on 23/09/2025.
//

#include "assets/AssetManager.h"

#include <fstream>
#include <assets/Material.h>
#include <assets/Model.h>
#include <assets/ShaderSource.h>
#include <assets/Texture.h>
#include <assets/Shader.h>
#include <assets/loaders/MaterialLoader.h>
#include <assets/loaders/ModelLoader.h>
#include <assets/loaders/ShaderLoader.h>
#include <assets/loaders/ShaderSourceLoader.h>
#include <assets/loaders/TextureLoader.h>

using namespace Motor;

// Singleton Setup
AssetManager* AssetManager::manager = nullptr;

AssetManager* AssetManager::getInstance() {
    if (!manager) {
        manager = new AssetManager();
        manager->init();
    }

    return manager;
}

void AssetManager::init() {
    // Register asset loaders
    assetLoaders[typeid(ShaderSource).name()] = std::make_unique<ShaderSourceLoader>();
    assetLoaders[typeid(Shader).name()] = std::make_unique<ShaderLoader>();
    assetLoaders[typeid(Texture).name()] = std::make_unique<TextureLoader>();
    assetLoaders[typeid(Material).name()] = std::make_unique<MaterialLoader>();
    assetLoaders[typeid(Model).name()] = std::make_unique<ModelLoader>();

    for (auto &loader : assetLoaders) {
        loader.second->registerDefaultAssets();
    }
}

bool AssetManager::isLoaded(const std::string &name) const {
    return loadedAssets.contains(name);
}

void AssetManager::setResourceFile(const std::string &path) {
    if (std::filesystem::exists(path))
        resourceLoader.initialize(path);
    else
        LOG_ERROR("Resource file not found under path '" + path + "'")
}

std::vector<char> AssetManager::readBytes(const std::string &path) const {
    return resourceLoader.getAsset(path);
}

BenzinLoader AssetManager::getLoader() { return resourceLoader; }

int AssetManager::getCacheCount() const {
    return loadedAssets.size();
}