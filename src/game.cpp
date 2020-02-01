#include "game.h"

#include "editor_window_system.h"

namespace Game
{
void Init()
{
    EditorWindowSystem::Init();
}

void Update()
{
    EditorWindowSystem::GetInstance()->Update();
}

void End()
{
    EditorWindowSystem::Destroy();
}
} // namespace Game