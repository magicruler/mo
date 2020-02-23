#version 420 core

out vec4 outColor;
in vec2 fragUV;

uniform sampler2D gBufferPosition;
uniform sampler2D gBufferNormalMetalness;
uniform sampler2D gBufferAlbedoSpecular;

void main() 
{
    vec3 albedoColor = texture(gBufferAlbedoSpecular, fragUV).xyz;
    outColor = vec4(albedoColor, 1.0);
}