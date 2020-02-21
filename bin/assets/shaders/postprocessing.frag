#version 420 core

out vec4 outColor;
in vec2 fragUV;

uniform float exposure;
uniform sampler2D hdrTarget;

vec3 ACESFilm(vec3 x)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

void main() 
{
    vec4 hdrColor = texture(hdrTarget, fragUV);

    // Tone Mapping
    vec3 mappedColor = vec3(1.0) - exp(-hdrColor.xyz * exposure);
    
    // Gamma Correction
    mappedColor = pow(mappedColor, vec3(1.0 / 2.2));

    outColor = vec4(mappedColor, 1.0);
}