#include "EditorWindowDock.h"

#include <imgui.h>
#include <imgui/Helpers.h>

#include "imgui/windows/TopToolbarWindow.h"
#include "imgui/windows/BottomToolbarWindow.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// EditorWindowDock
	//---------------------------------------------------------------------
	void EditorWindowDock::Render()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + TOP_TOOLBAR_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - TOP_TOOLBAR_HEIGHT - BOTTOM_TOOLBAR_HEIGHT));
		MainWindowDock::Render();
		ImGui::PopStyleVar();
	}

	//---------------------------------------------------------------------
	bool EditorWindowDock::OnInitialized()
	{
		return true;
	}

	//---------------------------------------------------------------------
	void EditorWindowDock::Update()
	{
		MainWindowDock::Update();
	}
}