//
// Created by Orik on 18/09/2025.
//

#ifndef MOTOR_SCENE_H
#define MOTOR_SCENE_H

#include <motor/assets/Material.h>

#include <memory>
#include "SceneNode.h"

namespace Motor {
    class Scene {
    public:
        Scene(const std::string &name);

        SceneNode* getRoot() const;

        void printTree();

        void init() const;
        void update(float deltaTime) const;
        void updateGlobalTransforms() const;

        // Skybox
        void setSkyboxColor(glm::vec3 color) const;
        glm::vec3 getSkyboxColor() const;

        void setSkyboxTexture(Texture* texture) const;

    private:
        std::string name;

        // Hold reference to root node
        std::unique_ptr<SceneNode> root;

        Material* const skybox;
    };
}

#endif //MOTOR_SCENE_H