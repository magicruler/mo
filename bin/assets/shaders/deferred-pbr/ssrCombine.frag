#version 420 core

out vec4 outColor;
in vec2 fragUV;

uniform sampler2D ssrPass;
uniform sampler2D lightPass;

void main() 
{
    outColor =  vec4(texture(ssrPass, fragUV).rgb + texture(lightPass, fragUV).rgb, 1.0);
}