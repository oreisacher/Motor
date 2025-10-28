//
// Created by Orik on 21/09/2025.
//

#include "assets/Material.h"

#include <assets/Shader.h>
#include <utils/UUIDHelper.h>

using namespace Motor;

glm::vec3 Material::getColor() const { return baseColor; }
void Material::setColor(const glm::vec3 &color) { this->baseColor = color; }

float Material::getMetallic() const { return metallic; }
void Material::setMetallic(float value) { this->metallic = value; }

float Material::getRoughness() const { return roughness; }
void Material::setRoughness(float value) { this->roughness = value; }

Shader* Material::getShader() const { return shader; }

ShaderInstance* Material::getShaderInstance() const { return shaderInstance.get(); }
void Material::setShaderInstance(const std::shared_ptr<ShaderInstance> &instance) {
    if (!shaderInstance)
        return;
    this->shaderInstance = instance;
}

std::unique_ptr<Material> Material::makeUnique() {
    auto newMaterial = std::make_unique<Material>(*this);
    auto newInstance = std::make_shared<ShaderInstance>(shaderInstance->getShader());

    // Copy current uniforms to copy
    for (const auto& [name, value] : shaderInstance->getUniforms()) {
        newInstance->setUniform(name, value);
    }

    newMaterial->setShaderInstance(newInstance);
    return newMaterial;
}

Texture* Material::getTexture(const std::string &name) const {
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;
    return nullptr;
}

std::vector<Texture*> Material::getTextures() const {
    std::vector<Texture*> textures;

    for (const auto& tex : this->textures)
        textures.push_back(tex.second);

    return textures;
}

std::vector<std::string> Material::getTextureNames() const {
    std::vector<std::string> textureNames;

    for (const auto& tex : this->textures)
        textureNames.push_back(tex.first);

    return textureNames;
}

int Material::getTextureCount() const {
    return textures.size();
}

float Material::getTextureTiling() const { return textureTiling; }
void Material::setTextureTiling(float value) { textureTiling = value; }

bool Material::hasDiffuseTexture() const { return diffuseTextureCount > 0; }
bool Material::hasMetallicTexture() const { return metallicTextureCount > 0; }
bool Material::hasRoughnessTexture() const { return roughnessTextureCount > 0; }
bool Material::hasAOTexture() const { return aoTextureCount > 0; }
bool Material::hasNormalTexture() const { return normalTextureCount > 0; }

void Material::addTexture(const std::string& name, Texture* texture) {
    textures[name] = texture;

    switch (texture->getTextureType()) {
        case Texture::DIFFUSE: diffuseTextureCount++; break;
        case Texture::METALLIC: metallicTextureCount++; break;
        case Texture::ROUGHNESS: roughnessTextureCount++; break;
        case Texture::AO: aoTextureCount++; break;
        case Texture::NORMAL: normalTextureCount++; break;
        default: break;
    }
}

const char* Material::getType() const { return "Material"; }