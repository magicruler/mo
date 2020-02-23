#version 420 core

out vec4 outColor;
in vec2 fragUV;

uniform sampler2D gBufferPosition;
uniform sampler2D gBufferNormalMetalness;
uniform sampler2D gBufferAlbedoRoughness;

void main() 
{
    vec3 albedoColor = texture(gBufferAlbedoRoughness, fragUV).xyz;
    vec3 N = texture(gBufferNormalMetalness, fragUV).xyz;
    float colorFactor = length(N);

    vec3 result = colorFactor * albedoColor;
    outColor = vec4(result, 1.0);
}