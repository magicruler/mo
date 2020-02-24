#version 420 core

out vec4 outColor;
in vec2 fragUV;

// camera space
uniform sampler2D gBufferPosition;
// camera space
uniform sampler2D gBufferNormalMetalness;
uniform sampler2D gBufferAlbedoRoughness;
// SSR Combine Pass
uniform sampler2D ssrCombine;

const float PI = 3.14159265;

// This Pass Should Output Non-Analytical Specular Term Based On Last Frame Before Post Processing
void main() 
{
    vec4 albedoRoughness = texture(gBufferAlbedoRoughness, fragUV);
    vec4 normalMetalness = texture(gBufferNormalMetalness, fragUV);

    float roughness = albedoRoughness.a;
    float metalness = normalMetalness.w; 
    vec3 position = texture(gBufferPosition, fragUV).xyz;
    vec3 N = normalMetalness.xyz;
    vec3 V = normalize(-position);

    float colorFactor = length(N);
    vec3 specular = vec3(0.0);

    vec3 result = colorFactor * (specular);

    outColor = vec4(result, 1.0);
}