#pragma once
#include "common.h"
#include "imgui.h"

class EditorWindow
{
public:
    EditorWindow(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : initialWidth(initialWidth),
                                                                                                               initialHeight(initialHeight),
                                                                                                               title(title),
                                                                                                               open(initialOpen),
                                                                                                               previousOpen(initialOpen)
    {
        Init();
    }

    ~EditorWindow()
    {
    }

    void OnFrame()
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

            ImGui::End();
        }
    }

protected:
    glm::vec2 contentMin;
    glm::vec2 contentMax;
    glm::vec2 contentSize;

private:
    virtual void Init()
    {
    }

    virtual void OnIMGUI()
    {
    }

    virtual void OnResize()
    {
    }

    virtual void OnOpen()
    {
        spdlog::info("OnOpen");
    }

    virtual void OnClose()
    {
        spdlog::info("OnClose");
    }

private:
    std::string title;

    unsigned int initialWidth;
    unsigned int initialHeight;

    bool firstFrame = true;
    bool previousOpen = true;
    bool open = true;
};