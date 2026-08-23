#include "BottomToolbarWindow.h"

#include "imgui/imgui.h"
#include "win32/Window.h"

#include "ui/AppLogger.h"

#include "editor/Workspace.h"

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

		ImVec2 textSize = ImGui::CalcTextSize(m_sMessage.c_str());
		ImVec2 textPos = {
			bottomToolbarStart.x + ImGui::GetStyle().WindowPadding.x,
			bottomToolbarStart.y + (BOTTOM_TOOLBAR_HEIGHT - textSize.y) * 0.5f
		};
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text("%s", m_sMessage.c_str());

		std::string version = "Humongous Entertainment Explorer " + GetWorkspace().GetAppVersion();
		ImVec2 versionTextSize = ImGui::CalcTextSize(version.c_str());
		ImVec2 versionTextPos = {
			bottomToolbarEnd.x - (ImGui::GetStyle().WindowPadding.x + versionTextSize.x),
			bottomToolbarStart.y + (BOTTOM_TOOLBAR_HEIGHT - textSize.y) * 0.5f
		};
		ImGui::SetCursorScreenPos(versionTextPos);
		ImGui::Text("%s", version.c_str());
	}

	//---------------------------------------------------------------------
	bool BottomToolbarWindow::OnInitialized()
	{
		logger::GetLogEvent() += std::bind(&BottomToolbarWindow::OnLogEvent, this, std::placeholders::_1, std::placeholders::_2);
		return true;
	}

	//---------------------------------------------------------------------
	void BottomToolbarWindow::OnLogEvent(core::LogLevel a_Level, const std::string& a_sMessage)
	{
		m_sMessage = a_sMessage;
	}
}
