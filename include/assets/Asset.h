//
// Created by Orik on 20/09/2025.
//

#ifndef MOTOR_ASSET_H
#define MOTOR_ASSET_H

#include <string>
#include <vector>
#include <memory>

namespace Motor {
    class Asset {
    public:
        Asset();
        virtual ~Asset() = default;

        // Id is immutable as they are used across the engine as a unique identifier
        const std::string& getId() const;

        const std::string& getName() const;
        void setName(const std::string& name);

        // Paths are immutable as they can only be set by an AssetLoader
        const std::string& getPath() const;

        virtual const char* getType() const;

    protected:
        std::string name;
        std::string path;

    private:
        const std::string id;
    };
}

#endif //MOTOR_ASSET_H