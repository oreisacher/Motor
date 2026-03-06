#ifndef MOTOR_SHADERINSTANCE_H
#define MOTOR_SHADERINSTANCE_H

#include <motor/utils/Logger.h>

#include <map>
#include <memory>
#include <any>
#include <optional>
#include <string>

namespace Motor {
    using UniformValue = std::any;

    class Shader;   // Note: Forward declaration because of circular imports

    class ShaderInstance {
    public:
        ShaderInstance(Shader* shader, bool defaultInstance = false);

        template<typename T>
        T getUniform(const std::string &name) {
            return std::any_cast<T>(uniforms.at(name));
        }

        template<typename T>
        void setUniform(const std::string &name, const T &value) {
            uniforms[name] = value;
        }

        bool hasUniform(const std::string &name) const;
        const std::map<std::string, UniformValue> &getUniforms() const;

        Shader* getShader() const;

        bool isDefault() const;
    private:
        Shader* shader;

        std::map<std::string, UniformValue> uniforms = {};
        bool defaultInstance = false;
    };
}

#endif //MOTOR_SHADERINSTANCE_H