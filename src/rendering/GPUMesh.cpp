//
// Created by Orik on 16/10/2025.
//

#include <motor/rendering/GPUMesh.h>
#include <glad/gl.h>

using namespace Motor;

GPUMesh::GPUMesh(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int instanceVBO) : vao(vao), vbo(vbo), ebo(ebo), instanceVBO(instanceVBO) { }

void GPUMesh::release() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteVertexArrays(1, &vao);
}
