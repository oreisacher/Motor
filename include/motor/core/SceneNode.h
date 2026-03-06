//
// Created by Orik on 18/09/2025.
//

#ifndef MOTOR_SCENENODE_H
#define MOTOR_SCENENODE_H

#include <motor/components/Component.h>

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Motor {
    class SceneNode {
    public:
        SceneNode();
        SceneNode(std::string name);
        virtual ~SceneNode() = default;

        std::string getName() const;
        void setName(const std::string &name);

        std::vector<Component*> getComponents() const;
        void addComponent(std::unique_ptr<Component> component);

        // Removes instance of component from node. Dangling pointer afterwards!
        void removeComponent(Component* component);

        // Removes components of type T
        template<typename T>
        void removeComponents();

        // Returns first component of type T
        template <typename T>
        T* getComponent();

        // Returns list of components of type T
        template <typename T>
        std::vector<T*> getComponents();

        SceneNode* getParent();

        int getChildCount() const;
        std::vector<SceneNode*> getChildren();
        SceneNode* getChild(const std::string &name);
        SceneNode* getChild(int index);
        void addChild(SceneNode *child);
        void removeChild(const SceneNode *child);
        void removeChild(int index);

        glm::vec3 getPosition() const;
        glm::vec3 getGlobalPosition() const;
        void setPosition(const glm::vec3 &position);

        glm::quat getRotation() const;
        glm::quat getGlobalRotation() const;
        void setRotation(const glm::quat &rotation);
        void setRotation(const glm::vec3 &rotation);

        glm::vec3 getScale() const;
        glm::vec3 getGlobalScale() const;
        void setScale(const glm::vec3 &scale);

        glm::mat4 getLocalTransform() const;
        glm::mat4 getGlobalTransform() const;

        void init();
        void update(float deltaTime);
        void updateGlobalTransform(const glm::mat4& parentGlobalTransform = glm::mat4(1.0f));

        virtual std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const SceneNode& node);

        void printTree(int depth, const std::string &prefix, bool isLast = true) const;

        virtual std::string getType() const;

    protected:
        // Transform
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        glm::vec3 globalPosition;
        glm::quat globalRotation;
        glm::vec3 globalScale;

        // Transform of node with respect to its parent
        glm::mat4 localTransform = glm::mat4(1.0f);

        // Global transform of node within scene graph
        glm::mat4 globalTransform = glm::mat4(1.0f);

        std::string name;
        std::vector<std::unique_ptr<Component>> components;

        SceneNode* parent = nullptr;
        std::vector<std::unique_ptr<SceneNode>> children;

        void updateLocalTransform();
    };

    template<typename T>
    void SceneNode::removeComponents() {
        std::erase_if(components, [&](const std::unique_ptr<Component>& c) {
            return dynamic_cast<T*>(c.get()) != nullptr;
        });
    }

    template<typename T>
    T* SceneNode::getComponent() {
        for (const auto& component : components) {
            if (auto casted = dynamic_cast<T*>(component.get())) {
                return casted;
            }
        }

        return nullptr;
    }

    template<typename T>
    std::vector<T*> SceneNode::getComponents() {
        std::vector<T*> results;

        for (const auto& component : components) {
            if (auto casted = dynamic_cast<T*>(component.get()))
                results.push_back(casted);
        }

        return results;
    }
}

#endif //MOTOR_SCENENODE_H