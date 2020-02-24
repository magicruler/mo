#version 420 core

out vec4 outColor;
noperspective in vec2 fragUV;

// camera space
uniform sampler2D gBufferPosition;
// camera space
uniform sampler2D gBufferNormalMetalness;
uniform sampler2D gBufferAlbedoRoughness;
// SSR Combine Pass
uniform sampler2D ssrCombine;

// uniform mat4 view;
// uniform mat4 invView;
uniform mat4 perspectiveProjection;
// uniform mat4 invProjection;

vec2 ViewSpaceToScreenSpace(vec3 viewPos)
{
    vec4 clipSpacePos = perspectiveProjection * vec4(viewPos, 1.0);
    vec3 ndcSpacePos = clipSpacePos.xyz / clipSpacePos.w;
    vec2 screenSpacePos = (ndcSpacePos.xy + vec2(1.0)) / 2.0;
    return screenSpacePos.xy;
}

// vec3 RayMarch(vec3 origin, vec3 direction)
// {
//     return vec3(0.0);
// }

// This Pass Should Output Non-Analytical Specular Term Based On Last Frame Before Post Processing
void main() 
{
    vec4 albedoRoughness = texture(gBufferAlbedoRoughness, fragUV);
    vec4 normalMetalness = texture(gBufferNormalMetalness, fragUV);

    float roughness = albedoRoughness.a;
    float metalness = normalMetalness.w; 

    // View Space
    vec3 position = texture(gBufferPosition, fragUV).xyz;
    vec3 N = normalMetalness.xyz;
    vec3 V = normalize(-position);
    float colorFactor = length(N);
    // perfect reflection vector 
    vec3 reflected = normalize(reflect(-V, N));
  
    vec3 result = vec3(0.0, 0.0, 0.0);
    // result = vec3(fragUV, 0.0);
    result = vec3(ViewSpaceToScreenSpace(position), 0.0);

    outColor = vec4(colorFactor * result, 1.0);
}