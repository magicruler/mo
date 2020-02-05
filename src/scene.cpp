#include "scene.h"
#include "actor.h"
#include "camera.h"
#include "light.h"
Scene::Scene()
{
	rootNode = new Actor();
}

Scene::~Scene()
{
	delete rootNode;
}

// Scene Traversal
std::vector<Actor*> sceneFrontLayer;
std::vector<Actor*> sceneBackLayer;

/*
This Vector Will Clear Every Frame, Will Be Used In Render Process
*/
std::vector<Light*> lights;

void Scene::Tick()
{	
	lights.clear();

	sceneFrontLayer = rootNode->GetChildren();

	while (sceneFrontLayer.size() != 0)
	{	
		sceneBackLayer.clear();
			
		for (int i = 0; i < sceneFrontLayer.size(); i++)
		{
			auto actor = sceneFrontLayer[i];
			actor->Tick();

			if (actor->GetHashID() == Light::GetHashIDStatic())
			{
				lights.push_back((Light*)actor);
			}

			for (int j = 0; j < actor->GetChildren().size(); j++)
			{
				sceneBackLayer.push_back(actor->GetChildren()[j]);
			}
		}

		sceneFrontLayer.swap(sceneBackLayer);
	}
}

std::vector<Light*> Scene::GetLights()
{
	return lights;
}