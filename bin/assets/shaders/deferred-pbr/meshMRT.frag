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

uniform float tillingX;
uniform float tillingY;

void main()
{
    float actualULength = 1.0 / tillingX;
    float actualVLength = 1.0 / tillingY;

    float xIndex = floor(uv.x / actualULength);
    float yIndex = floor(uv.y / actualVLength);

    vec2 actualUV = vec2((uv.x - xIndex * actualULength)/actualULength, (uv.y - yIndex * actualVLength)/actualVLength);

    float metalness = texture(metalnessMap, actualUV).r;
    float roughness = texture(roughnessMap, actualUV).r;

    // Attachment 0 CameraSpace
    gPosition = position;
    
    // Attachment 1
    vec3 mappedNormal = vec3(texture(normalMap, actualUV));
    mappedNormal = normalize(mappedNormal * 2.0 - 1.0);

    vec3 N = normalize(normal * mappedNormal.z + tangent * mappedNormal.x + bitangent * mappedNormal.y);
    gNormalMetalness = vec4(N, metalness);

    // Attachment 2
    vec3 diffuseColor = texture(albedoMap, actualUV).rgb;
    gAlbedoRoughness = vec4(diffuseColor, roughness);
}