#pragma once
#include "common.h"

class Scene;

class Texture;
class Material;
class Shader;

namespace Serialization
{
	void LoadProject();
	Scene* DeserializeScene(const std::string& content);
	/*
	Deserialize Texture Meta Data, Then Load Relavant Texture Images, Generate GPU Texture
	*/
	Texture* DeserializeTexture(const std::string& content);
	/*
	Deserialize Material Meta Data
	*/
	Material* DeserializeMaterial(const std::string& content);
	Shader* DeserializeShader(const std::string& content);
};