//
// Created by Orik on 16/10/2025.
//

#ifndef MOTOR_GPURESOURCEMANAGER_H
#define MOTOR_GPURESOURCEMANAGER_H

#include <motor/assets/Mesh.h>
#include <motor/assets/Shader.h>
#include <motor/assets/Texture.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "GPUMesh.h"
#include "GPUResource.h"
#include "GPUShader.h"
#include "GPUTexture.h"

namespace Motor {
    class GPUResourceManager {
    public:
        GPUShader* loadShader(const Shader* shader);
        GPUTexture* loadTexture(const Texture* texture);
        GPUMesh* loadMesh(const Mesh* mesh);

        void updateInstanceBuffer(GPUMesh* mesh, const std::vector<glm::mat4>& transforms);

    private:
        // Shader
        GPUShader* compileShader(const Shader* shader);
        unsigned int compileShaderSource(ShaderSource* source);

        // Texture
        GPUTexture* uploadTexture(const Texture* texture);

        // Mesh
        GPUMesh* uploadMesh(const Mesh* mesh);

        std::unordered_map<std::string, std::shared_ptr<GPUResource>> loaded;
    };
}

#endif //MOTOR_GPURESOURCEMANAGER_H