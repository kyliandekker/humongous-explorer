#include "HEBaseWindow.h"

#include <imgui.h>

#include "imgui/ImGuiSystem.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void HEBaseWindow::Render()
	{
		ImGui::PushFont(GetImGuiSystem().GetDefaultFont());
		BaseWindow::Render();
		ImGui::PopFont();
	}
}