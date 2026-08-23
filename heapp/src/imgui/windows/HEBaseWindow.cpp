#include "HEBaseWindow.h"

#include <imgui.h>

#include "imgui/ImGuiSystem.h"

#include "editor/Workspace.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void HEBaseWindow::Render()
	{
		ImGui::PushFont(GetImGuiSystem().GetDefaultFont());
		BaseWindow::Render();
		ImGui::PopFont();
	}

	//---------------------------------------------------------------------
	void LoggerDependentWindow::Render()
	{
		bool isOpen = GetWorkspace().IsLogHistoryPanelOpen();
		if (isOpen)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		}
		HEBaseWindow::Render();
		if (isOpen)
		{
			ImGui::PopItemFlag();
		}
	}
}