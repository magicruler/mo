#version 420 core

out vec4 outColor;
in vec2 fragUV;

void main() 
{
    outColor = vec4(fragUV, 0.0, 1.0);
}