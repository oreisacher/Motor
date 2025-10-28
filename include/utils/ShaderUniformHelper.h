//
// Created by Orik on 23/10/2025.
//

#ifndef MOTOR_SHADERUNIFORMHELPER_H
#define MOTOR_SHADERUNIFORMHELPER_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>

namespace Motor {
    inline void setUniformBool(unsigned int id, const std::string &name, bool value) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
    }
    inline void setUniformInt(unsigned int id, const std::string &name, int value) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    inline void setUniformFloat(unsigned int id, const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    inline void setUniformVec3(unsigned int id, const std::string &name, const glm::vec3 &value) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }
    inline void setUniformMat4(unsigned int id, const std::string &name, const glm::mat4 &value) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
}

#endif //MOTOR_SHADERUNIFORMHELPER_H