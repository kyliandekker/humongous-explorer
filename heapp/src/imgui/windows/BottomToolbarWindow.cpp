#include "BottomToolbarWindow.h"

#include "imgui/imgui.h"
#include "win32/Window.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// BottomToolbarWindow
	//---------------------------------------------------------------------
	BottomToolbarWindow::BottomToolbarWindow()
		: HEBaseWindow(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, "", "TOOLBAR", true)
	{
	}

	//---------------------------------------------------------------------
	void BottomToolbarWindow::Update()
	{
		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImVec2 bottomToolbarStart = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - BOTTOM_TOOLBAR_HEIGHT };
		ImVec2 bottomToolbarEnd = { bottomToolbarStart.x + ImGui::GetWindowSize().x, bottomToolbarStart.y + ImGui::GetWindowSize().y - BOTTOM_TOOLBAR_HEIGHT };
		drawlist->AddRectFilled(
			bottomToolbarStart,
			bottomToolbarEnd,
			IM_COL32(21, 26, 36, 255)
		);

		ImVec2 textSize = ImGui::CalcTextSize("Test");
		ImVec2 textPos = {
			bottomToolbarStart.x + ImGui::GetStyle().WindowPadding.x,
			bottomToolbarStart.y + (BOTTOM_TOOLBAR_HEIGHT - textSize.y) * 0.5f
		};
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text("Test");
	}

	//---------------------------------------------------------------------
	bool BottomToolbarWindow::OnInitialized()
	{
		return true;
	}
}
