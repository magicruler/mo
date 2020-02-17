#version 420 core

out vec4 outColor;

// worldSpace
in vec3 position;
// worldSpace
in vec3 normal;
// worldSpace
in vec2 uv;
// worldSpace
in vec3 tangent;
// worldSpace
in vec3 bitangent;

// CameraSapce
in vec3 positionCameraSpace;
// CameraSapce
in vec3 normalCameraSpace;
// CameraSapce
in vec3 tangentCameraSpace;
// CameraSapce
in vec3 bitangentCameraSpace;

struct Light
{
    vec3 Position;
    vec3 Color;
};

uniform Light lights[16];

uniform vec3 cameraPos;

uniform vec3 ambient;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform float glossiness;


void main() 
{
    vec3 mappedNormal = vec3(texture(normalMap, uv));
    mappedNormal = normalize(mappedNormal * 2.0 - 1.0);
    
    vec3 diffuseColor = vec3(texture(diffuseMap, uv));
    vec3 specularColor = vec3(texture(specularMap, uv));

    vec3 lambert = ambient * diffuseColor;
    vec3 specular = vec3(0.0);

    vec3 N = normalize(normal * mappedNormal.z + tangent * mappedNormal.x + bitangent * mappedNormal.y);
    vec3 V = normalize(cameraPos - position);

    for(int i = 0; i < 16; i++)
    {
        vec3 lightPos = lights[i].Position;
        vec3 lightColor = lights[i].Color;

        if(lightColor.x + lightColor.y + lightColor.z > 0.0)
        {
            vec3 L = normalize(lightPos - position);
            vec3 R = reflect(-L, N);
            vec3 H = (V + R) * 0.5;

            float HDotN = clamp(dot(H, N), 0.0, 1.0);
            float NDotL = clamp(dot(N, L), 0.0, 1.0);    

            vec3 lightIntensity = NDotL * lightColor;

            lambert += lightIntensity * diffuseColor;
            specular += lightColor * specularColor * pow(HDotN, glossiness); 
        }
    }
    
    outColor = vec4(lambert + specular, 1.0);
}