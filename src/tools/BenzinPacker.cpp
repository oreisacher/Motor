//
// Created by Orik on 25/09/2025.
//

#define MINIZ_HEADER_FILE_ONLY

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <zstd.h>

/*
    Benzin Asset Archive

    +---------------+-----------------------------+
    |   uint64_t    |   version                   |
    +---------------+-----------------------------+
    |   uint64_t    |   asset count               |
    +---------------+-----------------------------+
    | File entries (repeated for each file)       |
    |   uint64_t    |   filename length           |
    |   char[]      |   filename                  |
    |   uint64_t    |   offset to data            |
    |   uint64_t    |   size of compressed data   |
    |   uint64_t    |   size of original data     |
    +---------------+-----------------------------+
    | Compressed file data blocks (concatenated)  |
    |   char[]      |   compressed data           |
    +---------------+-----------------------------+

*/

uint64_t BENZIN_VERSION = 1;

struct Asset {
    std::string name;
    uint64_t offset;
    uint64_t compressedSize;
    uint64_t originalSize;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: AssetPacker <Asset Root Folder>" << std::endl;
        return 1;
    }

    std::vector<std::string> assetPaths;

    // Collect all files inside asset folder
    for (const auto& entry : std::filesystem::recursive_directory_iterator(argv[1])) {
        if (!entry.is_regular_file())
            continue;

        std::filesystem::path relativePath = std::filesystem::relative(entry.path(), argv[1]);
        assetPaths.push_back(relativePath.generic_string());
    }

    std::vector<Asset> entries;
    std::vector<std::vector<unsigned char>> fileCompressed;

    for (int i = 0; i < assetPaths.size(); i++) {
        // Asset root path + relative asset path
        std::filesystem::path path = std::filesystem::path(argv[1])/ assetPaths[i];

        // Open file in binary mode. Set Cursor to back of file
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file) {
            std::cerr << "Failed to open file " << path << std::endl;
            return 1;
        }

        // Cursor at back of file -> Corresponds to file size
        size_t originalSize = file.tellg();
        file.seekg(0);

        // Read file content
        std::vector<unsigned char> buffer(originalSize);
        file.read(reinterpret_cast<char *>(buffer.data()), originalSize);

        // Compress file
        size_t maxSize = ZSTD_compressBound(originalSize);
        std::vector<unsigned char> bufferCompressed(maxSize);

        std::cout << "Compressing " << assetPaths[i] << "..." << std::endl;
        size_t compressedSize = ZSTD_compress(
            bufferCompressed.data(),
            maxSize,
            buffer.data(),
            originalSize,
            3
        );

        if (ZSTD_isError(compressedSize)) {
            std::cerr << "Failed to compress file " << path << std::endl;
            std::cerr << ZSTD_getErrorName(compressedSize) << std::endl;
            return 1;
        }

        // Resize compressed buffer to actual compressed size
        bufferCompressed.resize(compressedSize);
        fileCompressed.push_back(bufferCompressed);

        entries.push_back({
            assetPaths[i],
            0,
            compressedSize,
            originalSize
        });
    }

    std::ofstream out("resources.benzin", std::ios::binary);

    if (!out) {
        std::cerr << "Failed to create benzin file" << std::endl;
        return 1;
    }

    // Write current benzin version
    out.write(reinterpret_cast<char*>(&BENZIN_VERSION), sizeof(BENZIN_VERSION));

    // Write file count
    uint64_t fileCount = entries.size();
    out.write(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));

    // Calculate offset to raw data
    uint64_t offset = 2 * sizeof(uint64_t);
    for (auto& e : entries) {
        //        filename length    filename        offset data        compressed size    original size
        offset += sizeof(uint64_t) + e.name.size() + sizeof(uint64_t) + sizeof(uint64_t) + sizeof(uint64_t);
    }

    // Write all file entries back to back
    for (int i = 0; i < fileCount; i++) {
        auto& entry = entries[i];
        entry.offset = offset;

        uint64_t filenameSize = entry.name.size();

        out.write(reinterpret_cast<char*>(&filenameSize), sizeof(filenameSize));
        out.write(entry.name.data(), filenameSize);
        out.write(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        out.write(reinterpret_cast<char*>(&entry.compressedSize), sizeof(entry.compressedSize));
        out.write(reinterpret_cast<char*>(&entry.originalSize), sizeof(entry.originalSize));

        // Move offset to new position
        offset += entry.compressedSize;
    }

    // Write raw file data
    for (int i = 0; i < entries.size(); i++) {
        std::cout << "Writing data of " << entries[i].name << "..." << std::endl;
        out.write(reinterpret_cast<char *>(fileCompressed[i].data()), fileCompressed[i].size());
    }

    out.close();
    std::cout << "Assets successfully packed into benzin file" << std::endl;
    return 0;
}