#include "game.h"

#include "editor_window_system.h"
#include "gameobject_system.h"
#include "component_system.h"
#include "scene_system.h"
#include "resource_system.h"

#include "test_object.h"
#include "event.h"

namespace Game
{
    void Init()
    {
        ResourceSystem::Init();
        GameObjectSystem::Init();
        ComponentSystem::Init();
        EditorWindowSystem::Init();
        SceneSystem::Init();

     /*    std::shared_ptr<TestObject> test = std::make_shared<TestObject>();
         test->RegisterEventListener<UpdateEvent>();

         auto updateEvent = std::make_shared<UpdateEvent>();
         Object::SendEvent(updateEvent);*/
    }

    void Update(float dt)
    {
        GameObjectSystem::GetInstance()->Update();
        ComponentSystem::GetInstance()->Update();
        EditorWindowSystem::GetInstance()->Update();
    }
}