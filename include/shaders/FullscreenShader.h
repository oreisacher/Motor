//
// Created by Orik on 25/10/2025.
//

#ifndef MOTOR_FULLSCREENSHADER_H
#define MOTOR_FULLSCREENSHADER_H

inline const char* fullscreenVertexShader = R"(
#version 430 core

out vec2 TexCoord;

const vec2 positions[4] = vec2[4](
    vec2(-1.0,  1.0),
    vec2(-1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2( 1.0, -1.0)
);

const vec2 uvs[4] = vec2[4](
    vec2(0.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0)
);

void main() {
    TexCoord = uvs[gl_VertexID];
    gl_Position = vec4(positions[gl_VertexID], 0.0f, 1.0f);
}
)";

#endif //MOTOR_FULLSCREENSHADER_H