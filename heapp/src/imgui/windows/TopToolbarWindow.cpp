#include "TopToolbarWindow.h"

#include <imgui/imgui.h>
#include <imgui/Helpers.h>
#include <imgui/font_icon.h>

#include "win32/Window.h"

#include "imgui/ImGuiSystem.h"

namespace humongousexplorer::imgui
{
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

		ImVec2 Size = ImGui::CalcTextSize(icon::ICON_MINUS);
		Size.y += ImGui::GetStyle().FramePadding.y * 2.0f;
		Size.x += ImGui::GetStyle().FramePadding.x * 2;
		Size.x *= 3;
		Size.x += ImGui::GetStyle().ItemSpacing.x * 2;

		Size.x += ImGui::GetStyle().WindowPadding.x;

		float buttonHeight = ImGui::GetFrameHeight();
		ImVec2 buttonPos = { topToolbarEnd.x - Size.x, topToolbarStart.y + (TOP_TOOLBAR_HEIGHT - Size.y) * 0.5f };
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

		TOP_TOOLBAR_HEIGHT = Size.y + (ImGui::GetStyle().WindowPadding.y * 2);
	}

	//---------------------------------------------------------------------
	bool TopToolbarWindow::OnInitialized()
	{
		return true;
	}
}
