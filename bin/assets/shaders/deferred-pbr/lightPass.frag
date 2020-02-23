#version 420 core

out vec4 outColor;
in vec2 fragUV;

uniform sampler2D gBufferPosition;
uniform sampler2D gBufferNormalMetalness;
uniform sampler2D gBufferAlbedoRoughness;

void main() 
{
    vec3 albedoColor = texture(gBufferAlbedoRoughness, fragUV).xyz;
    outColor = vec4(albedoColor, 1.0);
}