#include "scene.h"
#include "actor.h"

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

void Scene::Tick()
{	
	sceneFrontLayer = rootNode->GetChildren();

	while (sceneFrontLayer.size() != 0)
	{	
		sceneBackLayer.clear();
			
		for (int i = 0; i < sceneFrontLayer.size(); i++)
		{
			auto actor = sceneFrontLayer[i];
			actor->Tick();

			for (int j = 0; j < actor->GetChildren().size(); j++)
			{
				sceneBackLayer.push_back(actor->GetChildren()[j]);
			}
		}

		sceneFrontLayer.swap(sceneBackLayer);
	}
}