#pragma once
#include "common.h"

class RenderTarget;
class Scene;
class CommandBuffer;

namespace Game
{
void Init();
void Render();
void Update();
void PreRender();
void End();

void SetEnvironmentPath(const std::string& path);

// API
glm::vec2 MainRenderTargetGetSize();
RenderTarget* MainRenderTargetGetPointer();
Scene* ActiveSceneGetPointer();
CommandBuffer* GetCommandBuffer();
std::string GetEnviromentPath();
} // namespace Game