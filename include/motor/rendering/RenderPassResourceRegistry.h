//
// Created by Orik on 23/10/2025.
//

#ifndef MOTOR_RENDERPASSRESOURCEREGISTRY_H
#define MOTOR_RENDERPASSRESOURCEREGISTRY_H

#include <map>
#include <string>
#include <any>

namespace Motor {
    class RenderPassResourceRegistry {
    public:
        void set(const std::string& name, const unsigned int resource) {
            resources[name] = resource;
        }

        unsigned int get(const std::string& name) {
            auto it = resources.find(name);
            if (it == resources.end())
                return 0;
            return it->second;
        }

    private:
        std::map<std::string, unsigned int> resources;
    };
}

#endif //MOTOR_RENDERPASSRESOURCEREGISTRY_H