//
// Created by Orik on 25/09/2025.
//

#ifndef MOTOR_BENZINLOADER_H
#define MOTOR_BENZINLOADER_H
#include <map>
#include <string>
#include <vector>
#include <cstdint>

namespace Motor {
    struct BenzinAsset {
        std::string name;
        uint64_t offset;
        uint64_t compressedSize;
        uint64_t originalSize;
    };

    class BenzinLoader {
    public:
        void initialize(const std::string &path);
        std::vector<char> getAsset(const std::string &path) const;
        bool exists(const std::string &path) const;

        bool isInitialized() const;

    private:
        std::string path;
        std::map<std::string, BenzinAsset> resources;
        bool initialized = false;
    };
}

#endif //MOTOR_BENZINLOADER_H