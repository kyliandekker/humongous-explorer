#include "TopToolbarWindow.h"

#include <imgui/imgui.h>
#include <imgui/Helpers.h>
#include <imgui/font_icon.h>

#include "win32/Window.h"

#include "imgui/ImGuiSystem.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// TopToolbarWindow
	//---------------------------------------------------------------------
	TopToolbarWindow::TopToolbarWindow()
		: HEBaseWindow(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, "", "TOOLBAR", true)
	{
	}

	//---------------------------------------------------------------------
	void TopToolbarWindow::Update()
	{
		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImVec2 topToolbarStart = ImGui::GetWindowPos();
		ImVec2 topToolbarEnd = { topToolbarStart.x + ImGui::GetWindowWidth(), topToolbarStart.y + TOP_TOOLBAR_HEIGHT };
		drawlist->AddRectFilled(
			ImGui::GetWindowPos(), 
			ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), 
				ImGui::GetWindowPos().y + TOP_TOOLBAR_HEIGHT), 
			IM_COL32(21, 26, 36, 255)
		);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
		ImGui::PushFont(GetImGuiSystem().GetDefaultFont(), ImGui::GetFontSize() / 1.75f);

		float Size = ImGui::CalcTextSize(icon::ICON_MINUS).x;
		Size += ImGui::CalcTextSize(icon::ICON_MAXIMIZE).x;
		Size += ImGui::CalcTextSize(icon::ICON_CLOSE).x;
		Size += ImGui::GetStyle().FramePadding.x * 2.0f * 3;
		Size += ImGui::GetStyle().ItemSpacing.x * 2.0f;
		Size += ImGui::GetStyle().FramePadding.x;

		float buttonHeight = ImGui::GetFrameHeight();
		ImVec2 buttonPos = { topToolbarEnd.x - Size, topToolbarStart.y + (TOP_TOOLBAR_HEIGHT - buttonHeight) * 0.5f };
		ImGui::SetCursorScreenPos(buttonPos);
		if (ImGui::Button(FormatId(icon::ICON_MINUS, BUTTON_ID, "MINIMIZE").c_str()))
		{
			win32::GetWin32Window().Minimize();
		}
		ImGui::SameLine();
		if (ImGui::Button(FormatId(icon::ICON_MAXIMIZE, BUTTON_ID, "MAXIMIZE").c_str()))
		{
			win32::GetWin32Window().Maximize();
		}
		ImGui::SameLine();
		if (ImGui::Button(FormatId(icon::ICON_CLOSE, BUTTON_ID, "CLOSE").c_str()))
		{
			win32::GetWin32Window().Close();
		}

		ImGui::PopFont();
		ImGui::PopStyleVar();
	}

	//---------------------------------------------------------------------
	bool TopToolbarWindow::OnInitialized()
	{
		return true;
	}
}
