//
// Created by Orik on 25/10/2025.
//

#ifndef MOTOR_BLURSHADER_H
#define MOTOR_BLURSHADER_H

inline const char* blurFragmentShader = R"(
#version 430 core

uniform sampler2D renderTexture;
uniform int kernelSize = 21;       // Must be odd
uniform bool horizontal = true;   // true = horizontal, false = vertical

in vec2 TexCoord;
out vec4 FragColor;

const float PI = 3.14159265359;

void main() {
    float sigma = float(kernelSize) * 0.5 / 2.0;
    int center = kernelSize / 2;

    vec3 blurred = vec3(0.0);
    float weightSum = 0.0;

    ivec2 textureSize = textureSize(renderTexture, 0);

    for (int i = 0; i < kernelSize; i++) {
        float offset = float(i - center);
        float weight = (1.0 / (sqrt(2.0 * PI) * sigma)) * exp(-(offset * offset) / (2.0 * sigma * sigma));
        weightSum += weight;

        vec2 sampleUV = TexCoord;

        if (horizontal) {
            sampleUV.x += offset / float(textureSize.x);
        } else {
            sampleUV.y += offset / float(textureSize.y);
        }

        // Optional clamp to [0,1] to avoid sampling outside texture
        sampleUV = clamp(sampleUV, 0.0, 1.0);

        blurred += weight * texture(renderTexture, sampleUV).rgb;
    }

    FragColor = vec4(blurred / weightSum, 1.0);
}
)";

#endif //MOTOR_BLURSHADER_H