#version 420 core

layout (std140, binding = 0) uniform CameraBlock
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

// CameraSapce
out vec3 positionCameraSpace;

void main() 
{
    mat4 viewModel = view * model; 
    
    // camera space
    positionCameraSpace = vec3(viewModel * vec4(inPosition, 1.0));

    // screen space
    gl_Position = projection * viewModel * vec4(inPosition, 1.0);
}