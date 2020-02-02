#pragma once
#include "common.h"

class Shader;
class Texture;

enum class MaterialExtension
{
	MODEL_VIEW_PROJECTION,
	TIME,
};

class Material
{
public:
	Material(Shader* shader);

	void SetTextureProperty(const std::string& propName, Texture* texValue);
	void SetMatrix4(const std::string& propName, glm::mat4& matrix);
	void Use();

	void AddExtension(MaterialExtension extension)
	{
		extensions.push_back(extension);
	}

	std::vector<MaterialExtension> GetExtensions()
	{
		return extensions;
	}

public:
	std::string GetName()
	{
		return name;
	}

	void SetName(const std::string newName)
	{
		name = newName;
	}

private:
	std::string name = "";

	std::vector<MaterialExtension> extensions;
	std::map<std::string, Texture*> textureProperties;
	Shader* shader;
};

