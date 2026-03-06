//
// Created by Orik on 09/10/2025.
//

#ifndef MOTOR_MODEL_H
#define MOTOR_MODEL_H

#include <glm/glm.hpp>

#include "Animation.h"
#include "Asset.h"
#include "Mesh.h"

namespace Motor {
    class ModelInstance;

    class Model : public Asset {
    public:
        struct Node {
            std::string name;

            // Transform of node within model with respect to its parent node
            glm::mat4 localTransform;

            std::vector<unsigned int> meshes;
            std::vector<Node*> children;
        };

        ~Model() override;

        std::unique_ptr<ModelInstance> instantiate() const;

        Node* getRoot() const;

        // Meshes
        int getMeshCount() const;
        std::vector<Mesh*> getMeshes() const;
        Mesh* getMesh(int index) const;

        std::vector<Node*> getMeshNodes() const;

        // Material
        // Material* getMaterial(int meshIndex) const;
        // void setMaterial(int meshIndex, Material* material) const;

        // Animations
        int getAnimationCount() const;
        Animation* getAnimation(const std::string& name) const;
        std::vector<std::string> getAnimationNames() const;

        // Bone
        bool isSkinned() const;
        int getBone(const std::string &name) const;
        std::vector<glm::mat4> getBoneTransforms() const;
        glm::mat4 getBoneTransform(int index) const;

        const char* getType() const override;

    private:
        Node* root;
        std::vector<std::unique_ptr<Mesh>> meshes;

        // Keeps track of all nodes with meshes assigned. May be used for faster access during rendering.
        std::vector<Node*> meshNodes;

        // Bone data
        std::map<std::string, int> boneLookup;
        std::vector<glm::mat4> boneOffsetMatrices;

        // Animation data
        std::map<std::string, std::unique_ptr<Animation>> animations;

        // Materials
        std::map<std::string, Material*> defaultMaterials;

        void deleteNode(Node* node);

        friend class ModelInstance;
        friend class ModelLoader;
    };
};

#endif //MOTOR_MODEL_H