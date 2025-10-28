//
// Created by Orik on 09/10/2025.
//

#ifndef MOTOR_MODENODE_H
#define MOTOR_MODENODE_H

#include <assets/ModelInstance.h>
#include "SceneNode.h"

namespace Motor {
    class ModelNode : public SceneNode {
    public:
        ModelNode();
        ModelNode(const std::string &name);
        ModelNode(const std::string &name, ModelInstance* model);

        ModelInstance* getModelInstance() const;
        void setModelInstance(ModelInstance* model);

        std::string toString() const override;
        std::string getType() const override;
    private:
        ModelInstance* model;
    };
}

#endif //MOTOR_MOD