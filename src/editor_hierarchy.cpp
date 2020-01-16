#include "editor_hierarchy.h"
#include "scene_system.h"
#include "scene.h"
#include "gameobject.h"

EditorHierarchy::EditorHierarchy(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
{
}

void WindowMenu()
{
	if (ImGui::BeginPopupContextWindow("General"))
	{
		if (ImGui::MenuItem("Create"))
		{

		}

		ImGui::EndPopup();
	}
}

void TreeView()
{
	static ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	// bool selected = ImGuiTreeNodeFlags_Selected;

	Scene* currentScene = SceneSystem::GetInstance()->GetActivatedScene();
	auto rootList = currentScene->GetRootList();
	
	for (auto root : rootList)
	{
		if (ImGui::TreeNodeEx(root->GetName().c_str(), tree_flags))
		{
			if (ImGui::IsItemClicked())
			{

			}
		}
	}

}

void EditorHierarchy::OnIMGUI()
{
	WindowMenu();
	TreeView();
}

void EditorHierarchy::OnResize()
{
}