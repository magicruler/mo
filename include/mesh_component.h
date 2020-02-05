#pragma once
#include "common.h"
#include "component.h"

class Mesh;
class Material;

class MeshComponent: public Component
{
	MO_OBJECT("MeshComponent")
public:
	Mesh* mesh = nullptr;
	Material* material = nullptr;
	virtual void Clear();
};