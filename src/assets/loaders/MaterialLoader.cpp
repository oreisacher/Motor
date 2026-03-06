//
// Created by Orik on 23/09/2025.
//

#include <motor/assets/loaders/MaterialLoader.h>
#include <motor/utils/Logger.h>
#include <motor/assets/Shader.h>

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace Motor;

std::unique_ptr<Asset> MaterialLoader::load(const std::string &path) {
    std::unique_ptr<Material> material = std::make_unique<Material>();

    std::filesystem::path fsPath(path);

    if (fsPath.extension() != ".material") {
        LOG_ERROR("Unsupported material file")
        return nullptr;
    }

    material->name = fsPath.stem().string();
    material->path = path;

    LOG_INFO("Loading material '" + material->name + "' on path '" + path + "'")

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
        material->name = data["name"];
    }

    if (data.contains("baseColor")) {
        if (!data["baseColor"].is_array() || data["baseColor"].size() != 3) {
            LOG_ERROR("Base color requires 3 component array")
            return nullptr;
        }

        glm::vec3 color(1.0f);

        for (int i = 0; i < 3; i++) {
            if (data["baseColor"][i].is_number_float()) {
                color[i] = data["baseColor"][i].get<float>();
            } else {
                LOG_ERROR("Base color expects floats")
                return nullptr;
            }
        }

        material->baseColor = color;
    }

    if (data.contains("shader")) {
        if (!data["shader"].is_string()) {
            LOG_ERROR("Unsupported data type as shader path")
            return nullptr;
        }

        auto shader = AssetManager::getInstance()->load<Shader>(data["shader"].get<std::string>());
        if (!shader)
            return nullptr;

        material->shader = shader;
        material->shaderInstance = shader->getDefaultInstance();
    } else {
        LOG_ERROR("Material requires shader file")
        return nullptr;
    }

    if (data.contains("textures")) {
        if (!data["textures"].is_array()) {
            LOG_ERROR("Textures expects a array")
            return nullptr;
        }

        for (auto& dataTex : data["textures"]) {
            // Sampler
            if (!dataTex.contains("name")) {
                LOG_ERROR("Texture requires a name")
                return nullptr;
            }

            if (!dataTex["name"].is_string()) {
                LOG_ERROR("Unsupported data type as name")
                return nullptr;
            }

            // Type
            if (!dataTex.contains("type")) {
                LOG_ERROR("Texture requires a type")
                return nullptr;
            }

            if (!dataTex["name"].is_string()) {
                LOG_ERROR("Unsupported data type as type")
                return nullptr;
            }

            // Path
            if (!dataTex.contains("path")) {
                LOG_ERROR("Texture requires a path")
                return nullptr;
            }

            if (!dataTex["path"].is_string()) {
                LOG_ERROR("Unsupported data type as path")
                return nullptr;
            }

            auto texture = AssetManager::getInstance()->load<Texture>(dataTex["path"].get<std::string>());
            if (!texture)
                return nullptr;

            std::string type = dataTex["type"].get<std::string>();
            if (type == "diffuse") texture->setTextureType(Texture::DIFFUSE);
            if (type == "metallic") texture->setTextureType(Texture::METALLIC);
            if (type == "roughness") texture->setTextureType(Texture::ROUGHNESS);
            if (type == "ao") texture->setTextureType(Texture::AO);
            if (type == "normal") texture->setTextureType(Texture::NORMAL);
            if (type == "custom") texture->setTextureType(Texture::CUSTOM);

            material->addTexture(dataTex["name"], texture);
        }
    }

    return material;
}

void MaterialLoader::registerDefaultAssets() {
    auto skybox = new Material();
    skybox->name = "skybox";
    skybox->path = "motor://materials/skybox.material";
    skybox->shader = AssetManager::getInstance()->load<Shader>("motor://shaders/skybox.shader");
    skybox->shaderInstance = skybox->shader->getDefaultInstance();
    AssetManager::getInstance()->registerAsset(skybox->path, skybox);
}
