//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_RENDERCOMMAND_H
#define MOTOR_RENDERCOMMAND_H

#include <assets/Mesh.h>
#include <assets/Material.h>
#include <assets/Model.h>
#include <glm/glm.hpp>

namespace Motor {
    struct RenderCommand {
        const Mesh* mesh;
        const Material* material;

        // ModelMatrix should contain the actual model transform as well as the nodes global transform within the model itself
        std::vector<glm::mat4> modelTransforms;
    };
}

#endif //MOTOR_RENDERCOMMAND_H