#version 420 core

out vec4 outColor;
in vec2 fragUV;

uniform sampler2D renderTarget;

void main() 
{
    vec4 renderTargetColor = vec4(texture(renderTarget, fragUV).rgb, 1.0);
    outColor = renderTargetColor;
}