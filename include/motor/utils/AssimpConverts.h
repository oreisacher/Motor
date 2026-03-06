//
// Created by Orik on 20/08/2025.
//

#ifndef ASSIMPUTILS_H
#define ASSIMPUTILS_H

#include <assimp/vector3.h>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

constexpr float PI = 3.14159265358979323846f;

inline glm::vec3 aiVec3ToGlmVec3(const aiVector3D& vector) {
    return glm::vec3(vector.x, vector.y, vector.z);
}

inline glm::quat aiQuatToGlmQuat(const aiQuaternion& q) {
    return glm::quat(q.w, q.x, q.y, q.z);
}

inline glm::mat4 aiMatrix4x4ToGlmMat4(const aiMatrix4x4& t) {
    return glm::mat4(
        t.a1, t.b1, t.c1, t.d1,
        t.a2, t.b2, t.c2, t.d2,
        t.a3, t.b3, t.c3, t.d3,
        t.a4, t.b4, t.c4, t.d4
    );
}

#endif //ASSIMPUTILS_H
