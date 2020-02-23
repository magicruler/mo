#version 420 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormalMetalness;
layout (location = 2) out vec4 gAlbedoRoughness;

// worldSpace
in vec3 position;
// worldSpace
in vec3 normal;
// worldSpace
in vec2 uv;
// worldSpace
in vec3 tangent;
// worldSpace
in vec3 bitangent;

// CameraSapce
in vec3 positionCameraSpace;
// CameraSapce
in vec3 normalCameraSpace;
// CameraSapce
in vec3 tangentCameraSpace;
// CameraSapce
in vec3 bitangentCameraSpace;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metalnessMap;
uniform sampler2D roughnessMap;

void main()
{
    float metalness = texture(metalnessMap, uv).r;
    float roughness = texture(roughnessMap, uv).r;

    // Attachment 0
    gPosition = position;
    
    // Attachment 1
    vec3 mappedNormal = vec3(texture(normalMap, uv));
    mappedNormal = normalize(mappedNormal * 2.0 - 1.0);

    vec3 N = normalize(normal * mappedNormal.z + tangent * mappedNormal.x + bitangent * mappedNormal.y);
    gNormalMetalness = vec4(N, metalness);

    // Attachment 2
    vec3 diffuseColor = texture(albedoMap, uv).rgb;
    gAlbedoRoughness = vec4(diffuseColor, roughness);
}