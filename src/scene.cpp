#include "scene.h"
#include "actor.h"
#include "camera.h"

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
std::vector<Actor*> renderableActors;
/*
This Vector Will Clear Every Frame, Will Be Used In Render Process
*/
std::vector<Camera*> cameras;

void Scene::Tick()
{	
	renderableActors.clear();
	cameras.clear();

	sceneFrontLayer = rootNode->GetChildren();

	while (sceneFrontLayer.size() != 0)
	{	
		sceneBackLayer.clear();
			
		for (int i = 0; i < sceneFrontLayer.size(); i++)
		{
			auto actor = sceneFrontLayer[i];
			actor->Tick();

			if (actor->GetMaterial() != nullptr && actor->GetMesh() != nullptr)
			{
				renderableActors.push_back(actor);
			}
			
			if (actor->GetHashID() == Camera::GetHashIDStatic())
			{
				cameras.push_back((Camera*)actor);
			}

			for (int j = 0; j < actor->GetChildren().size(); j++)
			{
				sceneBackLayer.push_back(actor->GetChildren()[j]);
			}
		}

		sceneFrontLayer.swap(sceneBackLayer);
	}
}

std::vector<Camera*> Scene::GetCameras()
{
	return cameras;
}

std::vector<Actor*> Scene::GetRenderables()
{
	return renderableActors;
}
