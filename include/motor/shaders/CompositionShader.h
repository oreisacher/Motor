//
// Created by Orik on 25/10/2025.
//

#ifndef MOTOR_COMPOSITIONSHADER_H
#define MOTOR_COMPOSITIONSHADER_H

inline const char* compositionFragmentShader = R"(
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D renderTexture;
uniform sampler2D bloomTexture;

void main() {
    FragColor = texture(renderTexture, TexCoord) + texture(bloomTexture, TexCoord);
}
)";

#endif //MOTOR_COMPOSITIONSHADER_H