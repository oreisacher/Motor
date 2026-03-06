//
// Created by Orik on 09/10/2025.
//

#ifndef MOTOR_MESH_H
#define MOTOR_MESH_H

#include <motor/utils/Logger.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Material.h"

namespace Motor {
    struct Vertex {
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec2 TexCoord = glm::vec2(0.0f, 0.0f);

		glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);
    	glm::vec3 Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
    	glm::vec3 Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec4 Joints_0 = glm::vec4(-1.0f);
    	glm::vec4 Weights_0 = glm::vec4(0.0f);

		void addBoneData(float index, float weight) {
    		for (int i = 0 ; i < 4; i++) {
                // Check if slot is empty
                if (Joints_0[i] == -1.0f) {
                    Joints_0[i] = index;
                    Weights_0[i] = weight;
                    return;
                }
    		}

			LOG_ERROR("No more space remaining for bone data")
		}
    };

    // Note: Meshes can not be loaded from disk and are created as part of the model loading process.
    //		 Their id may be used by the renderer to uniquely identify the mesh.
    class Mesh : public Asset {
    public:
		Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

		std::vector<Vertex> getVertices() const;
    	std::vector<unsigned int> getIndices() const;

    private:
    	// Mesh data
    	std::vector<Vertex> vertices;
    	std::vector<unsigned int> indices;

    	friend class ModelLoader;
    };
};

#endif //MOTOR_MESH_H