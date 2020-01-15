#pragma once
#include "common.h"

class EditorWindow;

class EditorWindowSystem
{
public:
    static void Init();
    static void Destroy();

    static EditorWindowSystem *GetInstance();

    void Update();

    template <typename T>
    T* GetEditor()
    {
        size_t hashId = T::GetHashIDStatic();
        auto target = GetEditorById(hashId);
        if (target != nullptr)
        {
            return (T*)(target);
        }

        return nullptr;
    }

    EditorWindow* GetEditorById(size_t hashId);

    std::vector<EditorWindow*> editorWindows;

private:
    EditorWindowSystem();

    ~EditorWindowSystem();

    void InitWindows();

    static EditorWindowSystem *instance;
};
