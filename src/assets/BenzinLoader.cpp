//
// Created by Orik on 25/09/2025.
//

#define MINIZ_HEADER_FILE_ONLY

#include <motor/assets/BenzinLoader.h>
#include <motor/utils/Logger.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <zstd.h>

using namespace Motor;

void BenzinLoader::initialize(const std::string &path) {
    std::filesystem::path fsPath(path);

    // Check if path contains supported benzin file
    if (fsPath.extension() != ".benzin") {
        LOG_ERROR("Unsupported resource file")
        return;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        LOG_ERROR("Failed to open resource file '" + path + "'")
        return;
    }

    this->path = path;

    uint64_t version, assetCount;
    file.read(reinterpret_cast<char*>(&version), sizeof(uint64_t));
    file.read(reinterpret_cast<char*>(&assetCount), sizeof(uint64_t));

    LOG_INFO("Detected benzin version " + std::to_string(version))
    LOG_INFO("Loading " + std::to_string(assetCount) + " assets")

    for (int i = 0 ; i < assetCount ; i++) {
        BenzinAsset asset;

        // Read length of asset name
        uint64_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(uint64_t));

        // Read and assign asset name
        std::vector<char> assetName(nameLength);
        file.read(assetName.data(), nameLength);
        asset.name.assign(assetName.begin(), assetName.end());

        // Read offset, compressed size and uncompressed size
        file.read(reinterpret_cast<char*>(&asset.offset), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&asset.compressedSize), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&asset.originalSize), sizeof(uint64_t));

        resources[asset.name] = asset;

        LOG_INFO("[" + std::to_string(i + 1) + "/" + std::to_string(assetCount) + "] Loaded asset '" + asset.name + "'")
    }

    file.close();

    LOG_INFO("Resource file initialized");
    initialized = true;
}

std::vector<char> BenzinLoader::getAsset(const std::string &path) const {
    // Check if loader is initialized
    if (!initialized) {
        LOG_ERROR("BenzinLoader is not initialized");
        return {};
    }

    // Check if asset exists inside resource file
    auto it = resources.find(path);
    if (it == resources.end()) {
        LOG_ERROR("Asset '" + path + "' not found within resource file")
        return {};
    }

    auto entry = it->second;

    // Read compressed data
    std::ifstream file(this->path, std::ios::binary);
    if (!file) {
        LOG_ERROR("Failed to open file '" + this->path + "'")
        return {};
    }

    file.seekg(entry.offset);
    std::vector<char> compressedAsset(entry.compressedSize);
    file.read(compressedAsset.data(), entry.compressedSize);

    // Decompress data
    std::vector<char> uncompressedAsset(entry.originalSize);

    size_t decompressedSize = ZSTD_decompress(
        uncompressedAsset.data(),
        entry.originalSize,
        compressedAsset.data(),
        entry.compressedSize
    );

    if (ZSTD_isError(decompressedSize)) {
        LOG_ERROR("Failed to decompress asset '" + path + "'")
        LOG_INFO(std::string(ZSTD_getErrorName(decompressedSize)));
        return {};
    }

    file.close();
    return uncompressedAsset;
}

bool BenzinLoader::exists(const std::string &path) const {
    if (resources.find(path) == resources.end())
        return false;
    return true;
}

bool BenzinLoader::isInitialized() const {
    return initialized;
}
