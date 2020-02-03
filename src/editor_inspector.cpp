#include "editor_inspector.h"
#include "input.h"
#include "actor.h"
#include "editor_window_system.h"

EditorInspector::EditorInspector(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
{
}

void EditorInspector::OnIMGUI()
{
	auto selection = EditorWindowSystem::GetInstance()->GetActorSelection();
	if (selection.size() == 1)
	{
		auto actor = selection[0];
		if (actor != previousActor)
		{
			previousActor = actor;
			return;
		}

		spdlog::info("{} local position is: {}, {}, {}", actor->GetName(), actor->localPosition.x, actor->localPosition.y, actor->localPosition.z);
		if (actor->children.size() == 1)
		{
			spdlog::info("{} local position is: {}, {}, {}", actor->children[0]->GetName(),actor->children[0]->localPosition.x, actor->children[0]->localPosition.y, actor->children[0]->localPosition.z);
		}

		if (actor->parent != nullptr)
		{
			spdlog::info("{} local position is: {}, {}, {}", actor->parent->GetName(), actor->parent->localPosition.x, actor->parent->localPosition.y, actor->parent->localPosition.z);
		}

		if (ImGui::InputFloat3("Position", &actor->localPosition.x, 3))
		{
			actor->SetDirty();
		}

		if (ImGui::InputFloat3("Rotation", &actor->localRotation.x, 3))
		{
			actor->SetDirty();
		}

		if (ImGui::InputFloat3("Scale", &actor->localScale.x, 3))
		{
			actor->SetDirty();
		}
	}
}

void EditorInspector::OnResize()
{
}