#include "material.h"
#include "shader.h"

Material::Material(Shader* shader)
{
	this->shader = shader;
}