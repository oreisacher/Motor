//
// Created by Orik on 09/10/2025.
//

#include <motor/assets/BenzinLoader.h>
#include <motor/assets/BenzinAssimpAdapter.h>

#include <iostream>
#include <ostream>

using namespace Motor;

// ----- BENZINIOSTREAM -----

BenzinIOStream::BenzinIOStream(std::vector<char> data) : data(std::move(data)) { }

BenzinIOStream::~BenzinIOStream() = default;

size_t BenzinIOStream::Read(void *buffer, size_t size, size_t pCount) {
    size_t bytesToRead = std::min(size * pCount, data.size() - pos);
    std::memcpy(buffer, data.data() + pos, bytesToRead);
    pos += bytesToRead;
    return bytesToRead / size;
}

size_t BenzinIOStream::Write(const void *buffer, size_t size, size_t pCount) { return 0; }

aiReturn BenzinIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
    switch (pOrigin) {
        case aiOrigin_SET: pos = pOffset; break;
        case aiOrigin_CUR: pos += pOffset; break;
        case aiOrigin_END: pos = data.size() - pOffset; break;
        default: return aiReturn_FAILURE;
    }

    if (pos > data.size())
        pos = data.size();

    return aiReturn_SUCCESS;
}

size_t BenzinIOStream::Tell() const { return pos; }
size_t BenzinIOStream::FileSize() const { return data.size(); }
void BenzinIOStream::Flush() { }

// ----- BENZINIOSYSTEM -----

BenzinIOSystem::BenzinIOSystem(const BenzinLoader &loader, const std::string &path) {
    this->loader = std::move(loader);
    this->searchPath = path;
}

BenzinIOSystem::~BenzinIOSystem() = default;

bool BenzinIOSystem::Exists(const char* pFile) const {
    return loader.exists(searchPath + "/" + pFile);
}

char BenzinIOSystem::getOsSeparator() const {
    return '/';
}

Assimp::IOStream* BenzinIOSystem::Open(const char* pFile, const char* /*pMode*/) {
    auto data = loader.getAsset(searchPath + "/" + pFile);
    if (data.empty())
        return nullptr;
    return new BenzinIOStream(std::move(data));
}

void BenzinIOSystem::Close(Assimp::IOStream* pFile) {
    delete pFile;
}