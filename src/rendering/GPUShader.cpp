//
// Created by Orik on 16/10/2025.
//

#include <motor/rendering/GPUShader.h>
#include <glad/gl.h>

using namespace Motor;

GPUShader::GPUShader(unsigned int id) : id(id) { }

void GPUShader::release() {
    glDeleteProgram(id);
}
