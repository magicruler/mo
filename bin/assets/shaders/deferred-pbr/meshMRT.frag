#version 420 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormalMetalness;
layout (location = 2) out vec4 gAlbedoSpec;

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

void main()
{
    // Attachment 0
    gPosition = position;
    
    // Attachment 1
    gNormalMetalness = vec4(normalize(normal), 1.0);

    // Attachment 2
    vec3 diffuseColor = texture(albedoMap, uv).rgb;
    gAlbedoSpec = vec4(diffuseColor, 1.0);
}