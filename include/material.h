#pragma once
#include "common.h"

class Shader;
class Texture;

enum class MaterialExtension
{
	MODEL_VIEW_PROJECTION,
	TIME,
	AMBIENT,
	NEAREST_LIGHT,
	CAMERA
};

class Material
{
public:
	Material(Shader* shader);

	void SetTextureProperty(const std::string& propName, Texture* texValue);
	void SetMatrix4(const std::string& propName, glm::mat4& matrix);
	void SetFloat(const std::string& propName, float value);
	void SetInt(const std::string& propName, int value);
	void SetVector3(const std::string& propName, glm::vec3& value);
	void SetUniformBlock(const std::string& location, int index);

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
	
	std::map<std::string, float> floatProperties;
	std::map<std::string, glm::vec3> vec3Properties;
	std::map<std::string, int> intProperties;
	std::map<std::string, glm::mat4> mat4Properties;

	Shader* shader;
};

