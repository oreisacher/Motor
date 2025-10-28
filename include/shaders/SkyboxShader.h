//
// Created by Orik on 25/10/2025.
//

#ifndef MOTOR_SKYBOXSHADER_H
#define MOTOR_SKYBOXSHADER_H

inline const char* skyboxVertexShader = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 direction;

uniform mat4 view;
uniform mat4 projection;

void main() {
    mat4 viewNoTranslation = mat4(mat3(view)); // Remove camera translation
    direction = aPosition;
    vec4 pos = projection * viewNoTranslation * vec4(aPosition, 1.0);
    gl_Position = pos.xyww; // Force depth to 1.0
}
)";

inline const char* skyboxFragmentShader = R"(
#version 330 core
const float PI = 3.14159265359;

in vec3 direction;

out vec4 FragColor;

uniform vec3 color;

uniform bool hasSkybox;
uniform sampler2D skybox;

vec2 projectLongLat(vec3 direction) {
    float theta = atan(direction.x, -direction.z) + PI;
    float phi = acos(-direction.y);
    return vec2(theta / (2.0 * PI), phi / PI);
}

void main() {
    vec4 skyColor = vec4(color, 1.0f);

    if (hasSkybox) {
        skyColor *= textureLod(skybox, projectLongLat(normalize(direction)), 0.0);
    }

    // Tone mapping
    float exposure = 1.5f;
    skyColor = vec4(1.0f) - exp(-skyColor * exposure);

    FragColor = skyColor;
}
)";

#endif //MOTOR_SKYBOXSHADER_H