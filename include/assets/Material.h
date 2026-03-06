//
// Created by Orik on 21/09/2025.
//

#ifndef MOTOR_MATERIAL_H
#define MOTOR_MATERIAL_H

#include <memory>
#include <core/ShaderInstance.h>
#include <assets/Texture.h>
#include <glm/glm.hpp>
#include <map>

namespace Motor {
    class Material : public Asset {
    public:
        glm::vec3 getColor() const;
        void setColor(const glm::vec3& color);

        float getMetallic() const;
        void setMetallic(float value);

        float getRoughness() const;
        void setRoughness(float value);

        // Shader
        Shader* getShader() const;
        ShaderInstance* getShaderInstance() const;
        void setShaderInstance(const std::shared_ptr<ShaderInstance>& instance);

        std::unique_ptr<Material> makeUnique();

        // Textures
        Texture* getTexture(const std::string& name) const;
        std::vector<Texture*> getTextures() const;
        std::vector<std::string> getTextureNames() const;
        int getTextureCount() const;

        float getTextureTiling() const;
        void setTextureTiling(float value);

        bool hasDiffuseTexture() const;
        bool hasMetallicTexture() const;
        bool hasRoughnessTexture() const;
        bool hasAOTexture() const;
        bool hasNormalTexture() const;

        void addTexture(const std::string& name, Texture* texture);
        // TODO - Remove texture function

		const char* getType() const override;

    private:
        glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);

        float metallic = 0.0f;
        float roughness = 0.0f;

        Shader* shader;
        std::shared_ptr<ShaderInstance> shaderInstance = nullptr;

        std::map<std::string, Texture*> textures;

        float textureTiling = 1.0f;

        int diffuseTextureCount = 0;
        int metallicTextureCount = 0;
        int roughnessTextureCount = 0;
        int aoTextureCount = 0;
        int normalTextureCount = 0;

        friend class MaterialLoader;
        friend class ModelLoader;
    };
}

#endif //MOTOR_MATERIAL_H