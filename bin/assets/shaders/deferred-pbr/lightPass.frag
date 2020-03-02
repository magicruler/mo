#version 420 core

out vec4 outColor;
in vec2 fragUV;

// camera space
uniform sampler2D gBufferPosition;
// camera space
uniform sampler2D gBufferNormalMetalness;
uniform sampler2D gBufferAlbedoRoughness;

struct Light
{
    vec3 Position;
    vec3 Color;
    int Type;
    float SpotAngle;
    vec3 SpotDir;
};

uniform Light lights[16];

const float PI = 3.14159265;

void main() 
{
    vec4 albedoRoughness = texture(gBufferAlbedoRoughness, fragUV);
    vec4 normalMetalness = texture(gBufferNormalMetalness, fragUV);

    vec3 albedo = albedoRoughness.rgb;
    float roughness = albedoRoughness.a;
    vec3 N = normalMetalness.xyz;
    float metalness = normalMetalness.w; 
    vec3 position = texture(gBufferPosition, fragUV).xyz;
    vec3 V = normalize(-position);

    float colorFactor = length(N);

    vec3 lambert = vec3(0.0);
    vec3 specular = vec3(0.0);
    for(int i = 0; i < 4; i++)
    {
        vec3 lightPos = lights[i].Position;
        vec3 lightColor = lights[i].Color;
        int lightType = lights[i].Type;

        if(lightColor.x + lightColor.y + lightColor.z > 0.0)
        {
            vec3 L = normalize(lightPos - position);
            vec3 R = reflect(-L, N);
            vec3 H = (V + R) * 0.5;

            float HDotN = clamp(dot(H, N), 0.0, 1.0);
            float NDotL = clamp(dot(N, L), 0.0, 1.0);    

            vec3 lightIntensity = NDotL * lightColor;

            if(lightType == 1)
            {
                vec3 spotDir = normalize(lights[i].SpotDir);
                vec3 lightDir = -L;
                float spotDotLight = dot(spotDir, lightDir);
                if(spotDotLight < lights[i].SpotAngle)
                {
                    lightIntensity = vec3(0.0);
                }
            }

            float lightDistance = distance(lightPos, position);
            float falloff = 1.0 / (lightDistance * lightDistance);

            lambert += (falloff * lightIntensity * albedo / PI);
        }
    }

    vec3 result = colorFactor * (lambert + specular);

    outColor = vec4(result, 1.0);
}