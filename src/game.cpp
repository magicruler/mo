#include "game.h"

#include "editor_window_system.h"

namespace Game
{
void Init()
{
    EditorWindowSystem::Init();
}

void Update(float dt)
{
    EditorWindowSystem::GetInstance()->Update();
}

void End()
{
    EditorWindowSystem::Destroy();
}
} // namespace Game