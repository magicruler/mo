#include "editor_window.h"
#include "imgui.h"

EditorWindow::EditorWindow(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : initialWidth(initialWidth),
                                                                                                                         initialHeight(initialHeight),
                                                                                                                         title(title),
                                                                                                                         open(initialOpen),
                                                                                                                         previousOpen(initialOpen)
{
    Init();
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::OnFrame()
{
    if (previousOpen != open)
    {
        previousOpen = open;
        if (!open)
        {
            OnClose();
            return;
        }
        else
        {
            OnOpen();
        }
    }

    if (!open)
    {
        return;
    }

    ImGui::SetNextWindowSize(glm::vec2(initialWidth, initialHeight), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(title.c_str(), &open))
    {

        glm::vec2 windowPos = ImGui::GetWindowPos();
        contentMin = glm::vec2(ImGui::GetWindowContentRegionMin()) + windowPos;
        contentMax = glm::vec2(ImGui::GetWindowContentRegionMax()) + windowPos;

        if (firstFrame)
        {
            firstFrame = false;
            contentSize = contentMax - contentMin;
        }
        else
        {
            if (contentSize != contentMax - contentMin)
            {
                contentSize = contentMax - contentMin;
                OnResize();
            }
        }

        OnIMGUI();
    }

    ImGui::End();
}

void EditorWindow::Init()
{
}

void EditorWindow::OnIMGUI()
{
}

void EditorWindow::OnResize()
{
}

void EditorWindow::OnOpen()
{
    spdlog::info("OnOpen");
}

void EditorWindow::OnClose()
{
    spdlog::info("OnClose");
}