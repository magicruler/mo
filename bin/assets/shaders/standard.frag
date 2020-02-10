#version 420 core

out vec4 outColor;

in vec2 fragUV;
in vec2 fragTime;

uniform sampler2D albedoTex;
uniform sampler2D normalTex;

void main() 
{
    outColor = texture(albedoTex, fragUV);
}