#pragma once
#include "common.h"

class Light;
class Camera;
class MeshComponent;
class Mesh;
class Material;

class ComponentManager
{
public:
	static void Init();
	static void Destroy();

	static ComponentManager* GetInstance();

	/*
	Mesh Component
	*/
	MeshComponent* CreateMeshComponent(Mesh* mesh, std::vector<Material*>& materials);
	
	inline void AddToAvaliableMeshComponentsList(MeshComponent* meshCom)
	{
		meshComponents.remove(meshCom);
		meshComponentsAvaliable.push_back(meshCom);
	}

	inline std::list<MeshComponent*> GetMeshComponents() 
	{
		return meshComponents;
	}
	/*
	Camera
	*/
	Camera* CreateCameraComponent();

	inline void AddToAvaliableCameraComponentsList(Camera* cameraCom)
	{
		cameraComponents.remove(cameraCom);
		cameraComponentsAvaliable.push_back(cameraCom);
	}

	inline std::list<Camera*> GetCameraComponents()
	{
		return cameraComponents;
	}
	/*
	Light
	*/
	Light* CreateLightComponent();

	inline void AddToAvaliableLightComponentsList(Light* lightCom)
	{
		lightComponents.remove(lightCom);
		lightComponentsAvaliable.push_back(lightCom);
	}

	inline std::list<Light*> GetLightComponents()
	{
		return lightComponents;
	}

private:
	std::list<MeshComponent*> meshComponents;
	std::list<MeshComponent*> meshComponentsAvaliable;

	std::list<Camera*> cameraComponents;
	std::list<Camera*> cameraComponentsAvaliable;

	std::list<Light*> lightComponents;
	std::list<Light*> lightComponentsAvaliable;

private:
	ComponentManager();
	~ComponentManager();

	static ComponentManager* instance;
};