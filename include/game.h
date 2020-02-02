#pragma once
#include "common.h"

class RenderTarget;
class Scene;

namespace Game
{
void Init();
void Render();
void Update();
void End();

// API
glm::vec2 MainRenderTargetGetSize();
RenderTarget* MainRenderTargetGetPointer();
Scene* ActiveSceneGetPointer();
} // namespace Game