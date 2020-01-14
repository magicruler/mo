#pragma once
#include "common.h"

class EditorWindow;

class EditorWindowSystem
{
public:
    static void Init();

    static EditorWindowSystem *GetInstance();

    void Update();

    template <typename T>
    std::shared_ptr<T> GetEditor()
    {
        size_t hashId = T::GetHashIDStatic();
        auto target = GetEditorById(hashId);
        if (target != nullptr)
        {
            return std::dynamic_pointer_cast<T>(target);
        }

        return nullptr;
    }

    std::shared_ptr<EditorWindow> GetEditorById(size_t hashId);

    std::vector<std::shared_ptr<EditorWindow>> editorWindows;

private:
    EditorWindowSystem();

    ~EditorWindowSystem();

    void InitWindows();

    static EditorWindowSystem *instance;
};
