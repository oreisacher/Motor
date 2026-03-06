//
// Created by Orik on 21/09/2025.
//

#include <motor/assets/loaders/ShaderLoader.h>
#include <motor/utils/Logger.h>

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace Motor;

std::unique_ptr<Asset> ShaderLoader::load(const std::string &path) {
    std::unique_ptr<Shader> shader = std::make_unique<Shader>();

    std::filesystem::path fsPath(path);

    if (fsPath.extension() != ".shader") {
        LOG_ERROR("Unsupported shader file")
        return nullptr;
    }

    shader->name = fsPath.stem().string();
    shader->path = path;

    LOG_INFO("Loading shader '" + shader->name + "' on path '" + path + "'")

    auto rawData = AssetManager::getInstance()->readBytes(path);
    if (rawData.empty())
        return nullptr;

    std::string fileContent(rawData.begin(), rawData.end());

    json data;

    try {
        data = json::parse(fileContent);
    } catch (const json::parse_error &e) {
        LOG_ERROR("JSON parse error: \n\n" + std::string(e.what()) + "\n")
        return nullptr;
    } catch (const json::type_error &e) {
        LOG_ERROR("JSON type error: \n\n" + std::string(e.what()) + "\n")
        return nullptr;
    }

    // Overwrite name of shader if defined in json
    if (data.contains("name")) {
        shader->name = data["name"];
    }

    if (!data.contains("sources") || data["sources"].size() == 0) {
        LOG_ERROR("Shader does not contain sources")
        return nullptr;
    }

    for (auto& shaderSource : data["sources"]) {
        if (!shaderSource.is_string()) {
            LOG_ERROR("Unsupported data type within sources")
            return nullptr;
        }

        auto source = AssetManager::getInstance()->load<ShaderSource>(shaderSource.get<std::string>());
        if (!source)
            return nullptr;

        shader->addSource(source);
    }

    shader->defaultInstance = std::make_shared<ShaderInstance>(shader.get(), true);

    if (data.contains("uniforms")) {
        if (!data["uniforms"].is_array()) {
            LOG_ERROR("Expected array for uniform list")
            return nullptr;
        }

        for (auto& uniform : data["uniforms"]) {
            if (!uniform["name"].is_string()) {
                LOG_ERROR("Unsupported data type for uniform name")
                return nullptr;
            }

            std::string uniformName = uniform["name"].get<std::string>();

            if (!uniform["type"].is_string()) {
                LOG_ERROR("Unsupported data type for uniform name")
                return nullptr;
            }

            // Parse type
            if (uniform["type"].get<std::string>() == "float") {
                shader->defaultInstance->setUniform(uniformName, uniform["value"].get<float>());
                continue;
            }

            if (uniform["type"].get<std::string>() == "int") {
                shader->defaultInstance->setUniform(uniformName, uniform["value"].get<int>());
                continue;
            }

            if (uniform["type"].get<std::string>() == "vec2") {
                if (!uniform["value"].is_array() || uniform["value"].size() != 2) {
                    LOG_ERROR("Array with two elements expected for vec2")
                    return nullptr;
                }

                glm::vec2 vector(uniform["value"][0].get<float>(), uniform["value"][1].get<float>());
                shader->defaultInstance->setUniform(uniformName, vector);
                continue;
            }

            if (uniform["type"].get<std::string>() == "vec3") {
                if (!uniform["value"].is_array() || uniform["value"].size() != 3) {
                    LOG_ERROR("Array with three elements expected for vec3")
                    return nullptr;
                }

                glm::vec3 vector(uniform["value"][0].get<float>(), uniform["value"][1].get<float>(), uniform["value"][2].get<float>());
                shader->defaultInstance->setUniform(uniformName, vector);
                continue;
            }
        }
    }

    return shader;
}

void ShaderLoader::registerDefaultAssets() {
    auto manager = AssetManager::getInstance();

    auto defaultLit = new Shader();
    defaultLit->name = "DefaultLit";
    defaultLit->path = "motor://shaders/defaultLit.shader";
    defaultLit->addSource(manager->load<ShaderSource>("motor://shaders/defaultLitVert.ssrc"));
    defaultLit->addSource(manager->load<ShaderSource>("motor://shaders/defaultLitFrag.ssrc"));
    defaultLit->defaultInstance = std::make_shared<ShaderInstance>(defaultLit, true);
    manager->registerAsset(defaultLit->path, defaultLit);

    auto depth = new Shader();
    depth->name = "Depth";
    depth->path = "motor://shaders/depth.shader";
    depth->addSource(manager->load<ShaderSource>("motor://shaders/defaultLitVert.ssrc"));
    depth->addSource(manager->load<ShaderSource>("motor://shaders/emptyFrag.ssrc"));
    depth->defaultInstance = std::make_shared<ShaderInstance>(depth, true);
    manager->registerAsset(depth->path, depth);

    auto skybox = new Shader();
    skybox->name = "Skybox";
    skybox->path = "motor://shaders/skybox.shader";
    skybox->addSource(manager->load<ShaderSource>("motor://shaders/skyboxVert.ssrc"));
    skybox->addSource(manager->load<ShaderSource>("motor://shaders/skyboxFrag.ssrc"));
    skybox->defaultInstance = std::make_shared<ShaderInstance>(skybox, true);
    manager->registerAsset(skybox->path, skybox);

    auto composition = new Shader();
    composition->name = "Composition";
    composition->path = "motor://shaders/fullscreen.shader";
    composition->addSource(manager->load<ShaderSource>("motor://shaders/fullscreenVert.ssrc"));
    composition->addSource(manager->load<ShaderSource>("motor://shaders/compositionFrag.ssrc"));
    composition->defaultInstance = std::make_shared<ShaderInstance>(composition, true);
    manager->registerAsset(composition->path, composition);

    auto brightThreshold = new Shader();
    brightThreshold->name = "BrightThreshold";
    brightThreshold->path = "motor://shaders/brightThreshold.shader";
    brightThreshold->addSource(manager->load<ShaderSource>("motor://shaders/fullscreenVert.ssrc"));
    brightThreshold->addSource(manager->load<ShaderSource>("motor://shaders/brightThresholdFrag.ssrc"));
    brightThreshold->defaultInstance = std::make_shared<ShaderInstance>(brightThreshold, true);
    manager->registerAsset(brightThreshold->path, brightThreshold);

    auto blur = new Shader();
    blur->name = "Blur";
    blur->path = "motor://shaders/blur.shader";
    blur->addSource(manager->load<ShaderSource>("motor://shaders/fullscreenVert.ssrc"));
    blur->addSource(manager->load<ShaderSource>("motor://shaders/blurFrag.ssrc"));
    blur->defaultInstance = std::make_shared<ShaderInstance>(blur, true);
    manager->registerAsset(blur->path, blur);
}


