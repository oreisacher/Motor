//
// Created by Orik on 21/09/2025.
//

#include <motor/assets/AssetManager.h>
#include <motor/assets/loaders/TextureLoader.h>
#include <motor/utils/Logger.h>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Motor;

TextureLoader::TextureLoader() {
    stbi_set_flip_vertically_on_load(true);
}

std::unique_ptr<Asset> TextureLoader::load(const std::string &path) {
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();

    std::filesystem::path fsPath(path);
    texture->name = fsPath.stem().string();
    texture->path = path;

    LOG_INFO("Loading texture '" + texture->name + "' on path '" + path + "'")

    auto data = AssetManager::getInstance()->readBytes(path);
    if (data.empty())
        return nullptr;

    texture->data = stbi_load_from_memory(
        reinterpret_cast<stbi_uc const *>(data.data()),
        data.size(),
        &texture->width,
        &texture->height,
        &texture->nrChannels,
        0
    );

    if (!texture->data) {
        LOG_ERROR("Failed to load texture")
        return nullptr;
    }

    return texture;
}

void TextureLoader::registerDefaultAssets() { }
