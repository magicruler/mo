#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

out vec2 fragUV;
out vec2 fragTime;

void main() 
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    fragUV = inUV;
}