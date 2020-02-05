#include "editor_game.h"
#include "input.h"
#include "render_target.h"
#include "texture.h"
#include "game.h"

EditorGame::EditorGame(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
{
    gameViewRenderTarget = Game::MainRenderTargetGetPointer();
}

void EditorGame::OnIMGUI()
{
    if (!initialized)
    {
        initialized = true;
        gameViewRenderTarget->Resize(contentSize.x, contentSize.y);
    }
    else
    {
        auto textureId = gameViewRenderTarget->GetAttachmentTexture(0)->AsID();
        ImGui::Image((ImTextureID)textureId, gameViewRenderTarget->GetSize(), ImVec2(0, 1), ImVec2(1, 0));
    }
}

void EditorGame::OnResize()
{
    spdlog::info("Game Render Target Is {}", gameViewRenderTarget->GetGPUHandle());
    gameViewRenderTarget->Resize(contentSize.x, contentSize.y);
}