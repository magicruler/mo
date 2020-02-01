#pragma once
#include "common.h"

class Shader;

class Material
{
public:
	Material(Shader* shader);
private:
	Shader* shader;
};

