#include "editor_hierarchy.h"
#include "input.h"

EditorHierarchy::EditorHierarchy(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
{
}

void EditorHierarchy::WindowMenu()
{
	if (ImGui::BeginPopupContextWindow("General"))
	{
		if (ImGui::MenuItem("Create"))
		{
			// GameObject::Create();
		}

		ImGui::EndPopup();
	}
}

void EditorHierarchy::TreeView()
{
	// static ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	// // bool selected = ImGuiTreeNodeFlags_Selected;

	// Scene* currentScene = SceneSystem::GetInstance()->GetActivatedScene();
	// auto rootList = currentScene->GetRootList();
	// bool hoverAnyThing = false;

	// bool control_pressed = ImGui::GetIO().KeyCtrl;
	
	// for (auto root : rootList)
	// {
	// 	bool alreadyHad = false;
	// 	ImGuiTreeNodeFlags flag = tree_flags;
	// 	if (std::find(selection.begin(), selection.end(), root) != selection.end())
	// 	{
	// 		flag = tree_flags | ImGuiTreeNodeFlags_Selected;
	// 		alreadyHad = true;
	// 	}

	// 	ImGui::PushID(root->GetID());
	// 	bool treeItemOpen = ImGui::TreeNodeEx(root->GetName().c_str(), flag);
	// 	ImGui::PopID();

	// 	if (ImGui::IsItemHovered())
	// 	{
	// 		hoverAnyThing = true;
	// 	}

	// 	if (ImGui::IsItemClicked())
	// 	{
	// 		if (control_pressed)
	// 		{
	// 			selection.push_back(root);
	// 		}
	// 		else
	// 		{
	// 			selection.clear();
	// 			selection.push_back(root);
	// 		}
	// 	}

	// 	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	// 	{
	// 		if (!alreadyHad)
	// 		{
	// 			selection.clear();
	// 			selection.push_back(root);
	// 		}
	// 	}

	// 	if (treeItemOpen)
	// 	{
	// 		ImGui::TreePop();
	// 	}
	// }

	// if (!hoverAnyThing && ImGui::IsWindowFocused() && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
	// {
	// 	selection.clear();
	// }
}

void EditorHierarchy::OnIMGUI()
{
	WindowMenu();
	TreeView();
}

void EditorHierarchy::OnResize()
{
}