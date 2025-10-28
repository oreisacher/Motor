#include "core/ShaderInstance.h"

using namespace Motor;

ShaderInstance::ShaderInstance(Shader* shader, bool defaultInstance) :
    shader(shader),
    defaultInstance(defaultInstance)
{ }

bool ShaderInstance::hasUniform(const std::string &name) const {
    return uniforms.contains(name);
}

const std::map<std::string, UniformValue>& ShaderInstance::getUniforms() const { return uniforms; }

Shader* ShaderInstance::getShader() const { return shader; }

bool ShaderInstance::isDefault() const { return defaultInstance; }
