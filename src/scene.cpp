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