#include "HEBaseWindow.h"

#include <imgui.h>

#include "imgui/ImGuiSetup.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void HEBaseWindow::Render()
	{
		ImGui::PushFont(GetDefaultFont());
		BaseWindow::Render();
		ImGui::PopFont();
	}
}