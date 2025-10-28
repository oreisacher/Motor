//
// Created by Orik on 16/10/2025.
//

#include "rendering/GPUTexture.h"
#include <glad/gl.h>

using namespace Motor;

GPUTexture::GPUTexture(unsigned int id) : id(id) { }

void GPUTexture::release() {
    glDeleteTextures(1, &id);
}
