#version 420 core

out vec4 outColor;
in vec2 fragUV;

uniform sampler2D gBufferPosition;
uniform sampler2D gBufferNormalMetalness;
uniform sampler2D gBufferAlbedoRoughness;
uniform sampler2D gBufferDepth;
uniform sampler2D lightPass;
uniform sampler2D ssrPass;
uniform sampler2D ssrCombinePass;
uniform sampler2D backFacePass;
uniform sampler2D shadowMap;

// 1 albedo
// 2 position
// 3 normal
// 4 roughness
// 5 metalness
// 6 depth pass
// 7 light pass
// 8 ssr pass
// 9 ssr combine pass

uniform int debugOption;

void main() 
{
    outColor = vec4(0.0, 0.0, 0.0, 1.0);

    if(debugOption == 1)
    {
        vec3 albedoColor = texture(gBufferAlbedoRoughness, fragUV).xyz;
        outColor = vec4(albedoColor, 1.0);
    }
    else if(debugOption == 2)
    {
        vec3 positionColor = texture(gBufferPosition, fragUV).xyz;
        outColor = vec4(positionColor, 1.0);
    }
    else if(debugOption == 3)
    {
        vec3 normalColor = texture(gBufferNormalMetalness, fragUV).xyz * 0.5 + vec3(0.5, 0.5, 0.5);
        outColor = vec4(normalColor, 1.0);
    }
    else if(debugOption == 4)
    {
        float roughness = texture(gBufferAlbedoRoughness, fragUV).w;
        outColor = vec4(roughness,roughness,roughness, 1.0);
    }
    else if(debugOption == 5)
    {
        float metalness = texture(gBufferNormalMetalness, fragUV).w ;
        outColor = vec4(metalness, metalness, metalness, 1.0);
    }
    else if(debugOption == 6)
    {
        float depth = texture(gBufferNormalMetalness, fragUV).x;
        outColor = vec4(depth, depth, depth, 1.0);
    }
    else if(debugOption == 7)
    {
        outColor = vec4(texture(lightPass, fragUV).rgb, 1.0);
    }
    else if(debugOption == 8)
    {
        outColor = vec4(texture(ssrPass, fragUV).rgb, 1.0);
    }
    else if(debugOption == 9)
    {
        outColor = vec4(texture(ssrCombinePass, fragUV).rgb, 1.0);
    }
    else if(debugOption == 10)
    {
        outColor = vec4(-texture(backFacePass, fragUV).r, 0.0, 0.0, 1.0);
    }
    else if(debugOption == 11)
    {
        float positionColor = -texture(shadowMap, fragUV).x;
        outColor = vec4(positionColor, 0.0, 0.0, 1.0);
    }
}