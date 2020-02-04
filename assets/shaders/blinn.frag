#version 330 core

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

// world position
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 ambient;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float glossiness;

uniform vec3 cameraPos;

void main() 
{
    // world space
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPos - position);
    vec3 V = normalize(cameraPos - position);

    vec3 R = reflect(-L, N);
    vec3 H = (V + R) * 0.5;

    float HDotN = clamp(dot(H, N), 0.0, 1.0);
    float NDotL = clamp(dot(N, L), 0.0, 1.0);    

    vec3 lightIntensity = NDotL * lightColor;

    vec3 lambert = (lightIntensity + ambient) * diffuseColor;
    vec3 specular = lightColor * specularColor * pow(HDotN, glossiness); 

    outColor = vec4(lambert + specular, 1.0);
}