#include "light.h"
#include "actor.h"
#include "component_manager.h"

void Light::Clear()
{
	ComponentManager::GetInstance()->AddToAvaliableLightComponentsList(this);
}