#pragma once
#include "common.h"

namespace Game
{
void Init();
void Render();
void Update();
void End();

// API
glm::vec2 MainRenderTargetGetSize();
} // namespace Game