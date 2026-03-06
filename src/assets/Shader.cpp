//
// Created by Orik on 21/09/2025.
//

#include <motor/assets/Shader.h>
#include <iostream>
#include <ostream>

using namespace Motor;

ShaderSource* Shader::getSource(const ShaderSource::Type &type) {
    auto it = data.find(type);
    if (it == data.end())
        return nullptr;
    return it->second;
}

std::vector<ShaderSource*> Shader::getSources() const {
    std::vector<ShaderSource*> sources;
    sources.reserve(data.size());

    for (auto &src : data)
        sources.push_back(src.second);

    return sources;
}

void Shader::addSource(ShaderSource* shaderSource) {
    data[shaderSource->getShaderType()] = shaderSource;
}

void Shader::removeSource(const ShaderSource::Type &type) {
    data.erase(type);
}

std::shared_ptr<ShaderInstance> Shader::getDefaultInstance() { return defaultInstance; }

const char* Shader::getType() const { return "Shader"; }