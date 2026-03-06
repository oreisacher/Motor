//
// Created by Orik on 22/09/2025.
//

#include <motor/assets/AssetManager.h>
#include <motor/assets/loaders/AssetLoader.h>
#include <motor/assets/loaders/ShaderSourceLoader.h>

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>

#include <motor/shaders/DefaultShader.h>
#include <motor/shaders/FullscreenShader.h>
#include <motor/shaders/SkyboxShader.h>
#include <motor/shaders/BlurShader.h>
#include <motor/shaders/BrightThresholdShader.h>
#include <motor/shaders/CompositionShader.h>

using namespace Motor;

std::unique_ptr<Asset> ShaderSourceLoader::load(const std::string &path) {
    std::unique_ptr<ShaderSource> shaderSource = std::make_unique<ShaderSource>();

    std::filesystem::path fsPath(path);
    shaderSource->name = fsPath.stem().string();
    shaderSource->path = path;
    shaderSource->type = convert(fsPath.extension().string().erase(0, 1));

    LOG_INFO("Loading shader source '" + shaderSource->path + "'")
    auto data = AssetManager::getInstance()->readBytes(path);

    if (data.empty())
        return nullptr;

    shaderSource->data.assign(data.begin(), data.end());
    return shaderSource;
}

void ShaderSourceLoader::registerDefaultAssets() {
    auto defaultLitVertSource = new ShaderSource();
    defaultLitVertSource->name = "DefaultLitVert";
    defaultLitVertSource->path = "motor://shaders/defaultLitVert.ssrc";
    defaultLitVertSource->type = ShaderSource::VERTEX;
    defaultLitVertSource->data = defaultVertexShader;
    AssetManager::getInstance()->registerAsset(defaultLitVertSource->path, defaultLitVertSource);

    auto defaultLitFragSource = new ShaderSource();
    defaultLitFragSource->name = "DefaultLitFrag";
    defaultLitFragSource->path = "motor://shaders/defaultLitFrag.ssrc";
    defaultLitFragSource->type = ShaderSource::FRAGMENT;
    defaultLitFragSource->data = defaultFragmentShader;
    AssetManager::getInstance()->registerAsset(defaultLitFragSource->path, defaultLitFragSource);

    auto emptyFragSource = new ShaderSource();
    emptyFragSource->name = "EmptyLitFrag";
    emptyFragSource->path = "motor://shaders/emptyFrag.ssrc";
    emptyFragSource->type = ShaderSource::FRAGMENT;
    emptyFragSource->data = emptyFragmentShader;
    AssetManager::getInstance()->registerAsset(emptyFragSource->path, emptyFragSource);

    auto skyboxVertSource = new ShaderSource();
    skyboxVertSource->name = "SkyboxVert";
    skyboxVertSource->path = "motor://shaders/skyboxVert.ssrc";
    skyboxVertSource->type = ShaderSource::VERTEX;
    skyboxVertSource->data = skyboxVertexShader;
    AssetManager::getInstance()->registerAsset(skyboxVertSource->path, skyboxVertSource);

    auto skyboxFragSource = new ShaderSource();
    skyboxFragSource->name = "SkyboxFrag";
    skyboxFragSource->path = "motor://shaders/skyboxFrag.ssrc";
    skyboxFragSource->type = ShaderSource::FRAGMENT;
    skyboxFragSource->data = skyboxFragmentShader;
    AssetManager::getInstance()->registerAsset(skyboxFragSource->path, skyboxFragSource);

    auto fullscreenVertSource = new ShaderSource();
    fullscreenVertSource->name = "FullscreenVert";
    fullscreenVertSource->path = "motor://shaders/fullscreenVert.ssrc";
    fullscreenVertSource->type = ShaderSource::VERTEX;
    fullscreenVertSource->data = fullscreenVertexShader;
    AssetManager::getInstance()->registerAsset(fullscreenVertSource->path, fullscreenVertSource);

    auto compositionFragSource = new ShaderSource();
    compositionFragSource->name = "CompositionFrag";
    compositionFragSource->path = "motor://shaders/compositionFrag.ssrc";
    compositionFragSource->type = ShaderSource::FRAGMENT;
    compositionFragSource->data = compositionFragmentShader;
    AssetManager::getInstance()->registerAsset(compositionFragSource->path, compositionFragSource);

    auto brightThresholdFragSource = new ShaderSource();
    brightThresholdFragSource->name = "BrightThresholdFrag";
    brightThresholdFragSource->path = "motor://shaders/brightThresholdFrag.ssrc";
    brightThresholdFragSource->type = ShaderSource::FRAGMENT;
    brightThresholdFragSource->data = brightThresholdFragmentShader;
    AssetManager::getInstance()->registerAsset(brightThresholdFragSource->path, brightThresholdFragSource);

    auto blurFragSource = new ShaderSource();
    blurFragSource->name = "BlurFrag";
    blurFragSource->path = "motor://shaders/blurFrag.ssrc";
    blurFragSource->type = ShaderSource::FRAGMENT;
    blurFragSource->data = blurFragmentShader;
    AssetManager::getInstance()->registerAsset(blurFragSource->path, blurFragSource);
}

ShaderSource::Type ShaderSourceLoader::convert(const std::string &type) {
    static const std::unordered_map<std::string, ShaderSource::Type> types = {
        { "vert", ShaderSource::Type::VERTEX },
        { "frag", ShaderSource::Type::FRAGMENT },
        { "tesc", ShaderSource::Type::TESSELLATION_CONTROL },
        { "tese", ShaderSource::Type::TESSELLATION_EVAL },
        { "geom", ShaderSource::Type::GEOMETRY },
      };

    auto it = types.find(type);
    if (it != types.end())
        return it->second;

    LOG_FATAL("Unknown shader type '" + type + "'")
    return ShaderSource::Type::UNKNOWN;
}
