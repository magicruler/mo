#version 330 core

out vec4 outColor;

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

uniform int debugType;

void main() 
{
    vec3 normalCS = normalize(normalCameraSpace);
    outColor = vec4(normalCS * 0.5 + vec3(0.5), 1.0);
}