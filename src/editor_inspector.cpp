#include "editor_inspector.h"
#include "input.h"
#include "actor.h"
#include "editor_window_system.h"

EditorInspector::EditorInspector(unsigned int initialWidth, unsigned int initialHeight, bool initialOpen, std::string title) : EditorWindow(initialWidth, initialHeight, initialOpen, title)
{
}

void EditorInspector::OnIMGUI()
{
	/*auto selection = EditorWindowSystem::GetInstance()->GetActorSelection();
	if (selection.size() == 1)
	{
		auto actor = selection[0];
		if (actor != previousActor)
		{
			previousActor = actor;
			return;
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
	}*/
}

void EditorInspector::OnResize()
{
}