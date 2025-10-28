//
// Created by Orik on 09/10/2025.
//

#include "assets/Mesh.h"

using namespace Motor;

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
    this->vertices = vertices;
    this->indices = indices;
}

std::vector<Vertex> Mesh::getVertices() const { return vertices; }
std::vector<unsigned int> Mesh::getIndices() const { return indices; }