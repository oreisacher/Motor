//
// Created by Orik on 25/10/2025.
//

#ifndef MOTOR_BRIGHTTHRESHOLDSHADER_H
#define MOTOR_BRIGHTTHRESHOLDSHADER_H

inline const char* brightThresholdFragmentShader = R"(
#version 430 core

uniform sampler2D renderTexture;

in vec2 TexCoord;

out vec4 FragColor;

float grayScale(vec3 color) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main() {
    vec3 color = texture(renderTexture, TexCoord).rgb;
    float intensity = grayScale(color);

    if (intensity > 0.75f) {
        FragColor = vec4(color, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
)";

#endif //MOTOR_BRIGHTTHRESHOLDSHADER_H