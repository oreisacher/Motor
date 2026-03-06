//
// Created by Orik on 09/10/2025.
//

#ifndef MOTOR_BENZINASSIMPADAPTER_H
#define MOTOR_BENZINASSIMPADAPTER_H

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

namespace Motor {
    class BenzinIOStream : public Assimp::IOStream {
    public:
        BenzinIOStream(std::vector<char> data);
        ~BenzinIOStream() override;
        size_t Read(void *buffer, size_t size, size_t pCount) override;
        size_t Write(const void *buffer, size_t size, size_t pCount) override;
        aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;
        size_t Tell() const override;
        size_t FileSize() const override;
        void Flush() override;

    private:
        std::vector<char> data;
        size_t pos = 0;
    };

    class BenzinIOSystem : public Assimp::IOSystem {
    public:
        BenzinIOSystem(const BenzinLoader &loader, const std::string &path);
        ~BenzinIOSystem() override;

        bool Exists(const char *pFile) const override;
        char getOsSeparator() const override;
        Assimp::IOStream* Open(const char *pFile, const char *pMode) override;
        void Close(Assimp::IOStream *pFile) override;
    private:
        BenzinLoader loader;
        std::string searchPath;
    };
};

#endif //MOTOR_BENZINASSIMPADAPTER_H