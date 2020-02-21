#version 420 core
out vec4 FragColor;

in vec3 uv;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, uv);
}