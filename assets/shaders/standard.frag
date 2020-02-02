#version 330 core

uniform sampler2D albedoTex;
uniform sampler2D normalTex;

in vec2 fragUV;
in vec2 fragTime;

out vec4 outColor;

void main() 
{
    outColor = texture(albedoTex, fragUV);
}