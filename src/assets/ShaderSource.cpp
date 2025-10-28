//
// Created by Orik on 22/09/2025.
//

#include "assets/ShaderSource.h"

using namespace Motor;

const std::string & ShaderSource::getData() { return data;}

const ShaderSource::Type& ShaderSource::getShaderType() const { return type; }

const char* ShaderSource::getType() const { return "ShaderSource"; }
