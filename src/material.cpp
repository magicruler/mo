#include "material.h"
#include "shader.h"
#include "texture.h"

Material::Material(Shader* shader)
{
	this->shader = shader;
}

void Material::SetTextureProperty(const std::string& propName, Texture* texValue)
{
	textureProperties[propName] = texValue;
}

void Material::SetMatrix4(const std::string& propName, glm::mat4& matrix)
{
	shader->SetMat4(propName, matrix);
}

void Material::Use()
{
	/*
	Bind Textures
	*/
	int unitIndex = 0;
	for (auto& item : textureProperties)
	{
		shader->SetInt(item.first, unitIndex);

		item.second->Bind(unitIndex);
		unitIndex += 1;
	}

	shader->Use();
}