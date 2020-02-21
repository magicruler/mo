#version 420 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

uniform mat4 model;
uniform mat4 projection;

out vec2 fragUV;

void main() 
{
    gl_Position = projection * model * vec4(inPosition, 0.0, 1.0);
    fragUV = vec2(inUV.x, 1.0 - inUV.y);
}