#pragma once
#include "common.h"

class MeshComponent;
class Mesh;
class Material;

class ComponentManager
{
public:
	static void Init();
	static void Destroy();

	static ComponentManager* GetInstance();

	MeshComponent* CreateMeshComponent(Mesh* mesh, Material* material);
	
	inline void AddToAvaliableMeshComponentsList(MeshComponent* meshCom)
	{
		meshComponents.remove(meshCom);
		meshComponentsAvaliable.push_back(meshCom);
	}

	inline std::list<MeshComponent*> GetMeshComponents() 
	{
		return meshComponents;
	}

private:
	std::list<MeshComponent*> meshComponents;
	std::list<MeshComponent*> meshComponentsAvaliable;

private:
	ComponentManager();
	~ComponentManager();

	static ComponentManager* instance;
};