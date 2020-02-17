#version 420 core

out vec4 outColor;

in vec2 fragUV;
in vec2 fragTime;

uniform sampler2D albedoTex;

void main() 
{
    outColor = texture(albedoTex, fragUV);
}