//
// Created by Orik on 09/10/2025.
//

#pragma once

inline const char* defaultVertexShader = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (location = 5) in vec4 aJoints;
layout (location = 6) in vec4 aWeights;

layout (location = 7) in mat4 aInstanceModel;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 view;
uniform mat4 projection;

uniform bool instanced;

uniform bool skinned;
uniform mat4 u_jointMat[200];

void main() {
    // --- Skinning ---
    mat4 skinMat = mat4(1.0f);
    if (skinned) {
        skinMat = aWeights.x * u_jointMat[int(aJoints.x)];
        skinMat += aWeights.y * u_jointMat[int(aJoints.y)];
        skinMat += aWeights.z * u_jointMat[int(aJoints.z)];
        skinMat += aWeights.w * u_jointMat[int(aJoints.w)];
    }

    // --- TBN Matrix ---
    mat3 normalMatrix = mat3(transpose(inverse(aInstanceModel * skinMat)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    Normal = normalize(normalMatrix * aNormal);
    TBN = mat3(T, B, Normal);

    FragPos = vec3(aInstanceModel * skinMat * vec4(aPosition, 1.0));
    TexCoord = aTexCoord;

    gl_Position = projection * view * aInstanceModel * skinMat * vec4(aPosition, 1.0);
}
)";

inline const char* defaultFragmentShader = R"(
#version 330 core

#define MAX_LIGHTS 8
#define PCF_SIZE 5

const float PI = 3.14159265359;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in mat3 TBN;

out vec4 FragColor;

uniform vec3 viewPos;

// Material
uniform vec3 baseColor;
uniform float metallic;
uniform float roughness;

uniform float tiling;

uniform bool hasDiffuse;
uniform sampler2D diffuse0;

uniform bool hasMetallic;
uniform sampler2D metallic0;

uniform bool hasRoughness;
uniform sampler2D roughness0;

uniform bool hasAO;
uniform sampler2D ao0;

uniform bool hasNormal;
uniform sampler2D normal0;

// Environment
uniform vec3 skyColor;

uniform bool hasSkybox;
uniform sampler2D skybox;

struct Light {
    int type;   // 0 = directional, 1 = point, 2 = spot
    vec3 color;
    float intensity;

    vec3 position;
    vec3 direction;

    mat4 view;
    mat4 projection;

    float linear;
    float quadratic;

    float innerAngle;
    float outerAngle;

    sampler2D shadowMap;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform float shadowMapResolution;

vec2 projectLongLat(vec3 direction) {
    direction = normalize(direction);

    float theta = atan(direction.x, -direction.z);
    float phi = acos(clamp(-direction.y, -1.0, 1.0));

    float u = (theta + PI) / (2.0 * PI);
    u = fract(u);
    float v = phi / PI;

    return vec2(u, v);
}

// --- Shadow Mapping ---
float checkShadow(int index, vec3 lightDir, vec3 normal) {
    float texelSize = 1.0f / shadowMapResolution;

    vec4 fragPosLightSpace = lights[index].projection * lights[index].view * vec4(FragPos, 1.0f);

    // Check if in shadow
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;

    float currentDepth = projCoords.z;

    float bias = max(0.005f * (1.0f - dot(normalize(normal), lightDir)), 0.0005f);
    float shadow = 0.0f;

    int start = -int((PCF_SIZE - 1) / 2);
    int end = start + PCF_SIZE;

    for (int x = start ; x < end ; x++) {
        for (int y = start ; y < end ; y++) {
            float closestDepth = texture(lights[index].shadowMap, projCoords.xy + texelSize * vec2(x, y)).r;
            shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
        }
    }

    return shadow / float(PCF_SIZE * PCF_SIZE);
}

// --- PBR ---
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N,H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r*r)/8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    return GeometrySchlickGGX(max(dot(N,V),0.0), roughness) * GeometrySchlickGGX(max(dot(N,L),0.0), roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 calculateLighting(
    int index,
    vec3 lightDir,
    vec3 lightColor,
    vec3 normal,
    vec3 viewDir,
    vec3 albedo,
    float metallic,
    float roughness,
    float ao,
    float attenuation,
    float intensity
) {
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 H = normalize(viewDir + lightDir);

    float NDF = DistributionGGX(normal, H, roughness);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);
    vec3 F = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denom = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
    vec3 specular = numerator / denom;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 Lo = (kD * albedo / PI + specular) * lightColor * NdotL * intensity * attenuation;

    vec3 ambient = vec3(0.03) * albedo * ao;

    float shadow = checkShadow(index, lightDir, normal);
    return ambient + (1.0 - shadow) * Lo;
}

vec3 calcLight(int index, vec3 normal, vec3 viewDir, float metallic, float roughness, float ao) {
    vec3 albedo = baseColor;
    if (hasDiffuse) {
        albedo *= texture(diffuse0, TexCoord * tiling).rgb;
    }

    vec3 lightDir;
    float attenuation = 1.0;
    float intensity = 1.0;
    float dist;

    switch (lights[index].type) {
        case 0: // Directional
            lightDir = normalize(-lights[index].direction);
            return calculateLighting(index, lightDir, lights[index].color, normal, viewDir, albedo, metallic, roughness, ao, 1.0, 1.0);

        case 1: // Point
            lightDir = normalize(lights[index].position - FragPos);
            dist = length(lights[index].position - FragPos);
            attenuation = 1.0 / (1.0 + lights[index].linear * dist + lights[index].quadratic * dist * dist);

            return calculateLighting(index, lightDir, lights[index].color, normal, viewDir, albedo, metallic, roughness, ao, attenuation, 1.0);

        case 2: // Spot
            lightDir = normalize(lights[index].position - FragPos);
            dist = length(lights[index].position - FragPos);
            attenuation = 1.0 / (1.0 + lights[index].linear * dist + lights[index].quadratic * dist * dist);

            float theta = dot(lightDir, normalize(-lights[index].direction));
            float epsilon = lights[index].innerAngle - lights[index].outerAngle;
            intensity = clamp((theta - lights[index].outerAngle) / epsilon, 0.0, 1.0);

            return calculateLighting(index, lightDir, lights[index].color, normal, viewDir, albedo, metallic, roughness, ao, attenuation, intensity);
    }

    return vec3(0.0);
}

void main() {
    float m = hasMetallic ? texture(metallic0, TexCoord * tiling).b : metallic;
    float r = hasRoughness ? texture(roughness0, TexCoord * tiling).g : roughness;
    float ao = hasAO ? texture(ao0, TexCoord * tiling).r : 1.0f;

    // --- Normals ---
    vec3 norm;

    if (hasNormal) {
        norm = texture(normal0, TexCoord * tiling).rgb;
        norm = norm * 2.0 - 1.0;
        norm = normalize(TBN * norm);
    } else {
        norm = normalize(Normal);
    }

    // --- Lighting ---
    vec3 color = vec3(0.0f);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0 ; i < numLights ; i++) {
        color += calcLight(i, norm, viewDir, m, r, ao);
    }

    if (hasSkybox) {
        vec3 R = reflect(-viewDir, norm);
        vec3 envColor = texture(skybox, projectLongLat(R)).rgb;

        vec3 F0 = mix(vec3(0.04f), baseColor, metallic);
        vec3 F = fresnelSchlick(max(dot(norm, viewDir), 0.0f), F0);

        float reflectionAmount = (1.0 - roughness) * (0.1 + 0.5 * metallic);
        color += envColor * F * reflectionAmount;
    }

    // Tone mapping
    float exposure = 1.5f;
    color = vec3(1.0f) - exp(-color * exposure);

    FragColor = vec4(color, 1.0f);
};
)";

inline const char* emptyFragmentShader = R"(
#version 330 core

void main() { }
)";