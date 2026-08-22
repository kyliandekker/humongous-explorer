#include "Theme.h"

#include <imgui/imgui.h>
#include <imgui/implot.h>

#include "imgui/ImGuiSystem.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void ApplyTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		//----------------------------------------------------------
		// Layout
		//----------------------------------------------------------

		style.WindowPadding = ImVec2(10, 10);
		style.FramePadding = ImVec2(17, 17);
		style.CellPadding = ImVec2(8, 6);
		style.ItemSpacing = ImVec2(8, 8);
		style.ItemInnerSpacing = ImVec2(6, 4);

		style.TouchExtraPadding = ImVec2(0, 0);

		style.IndentSpacing = 20.0f;
		style.ScrollbarSize = 13.0f;
		style.GrabMinSize = 10.0f;

		//----------------------------------------------------------
		// Borders
		//----------------------------------------------------------

		style.WindowBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.FrameBorderSize = 1.0f;
		style.TabBorderSize = 0.0f;

		//----------------------------------------------------------
		// Rounding
		//----------------------------------------------------------

		style.WindowRounding = 8.0f;
		style.ChildRounding = 8.0f;
		style.FrameRounding = 6.0f;
		style.PopupRounding = 8.0f;
		style.ScrollbarRounding = 10.0f;
		style.GrabRounding = 6.0f;
		style.TabRounding = 6.0f;

		//----------------------------------------------------------
		// Alignment
		//----------------------------------------------------------

		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.5f);

		//----------------------------------------------------------
		// Colors
		//----------------------------------------------------------

		ImVec4* colors = style.Colors;

		auto C = [](int r, int g, int b, int a = 255)
			{
				return ImVec4(
					r / 255.0f,
					g / 255.0f,
					b / 255.0f,
					a / 255.0f);
			};

		//==========================================================
		// Text
		//==========================================================

		colors[ImGuiCol_Text] = C(209, 211, 212);
		colors[ImGuiCol_TextDisabled] = C(135, 145, 165);

		//==========================================================
		// Frames
		//==========================================================

		ImColor fr = C(24, 35, 47);
		colors[ImGuiCol_FrameBg] = C(20, 30, 41);
		colors[ImGuiCol_FrameBgHovered] = C(55, 67, 90);
		colors[ImGuiCol_FrameBgActive] = C(80, 60, 135);

		//==========================================================
		// Windows
		//==========================================================

		colors[ImGuiCol_WindowBg] = fr;
		colors[ImGuiCol_ChildBg] = C(28, 35, 47);
		colors[ImGuiCol_PopupBg] = C(33, 40, 55);

		//==========================================================
		// Borders
		//==========================================================

		colors[ImGuiCol_Border] = C(44, 54, 69);
		colors[ImGuiCol_BorderShadow] = C(0, 0, 0, 0);

		//==========================================================
		// Title
		//==========================================================

		colors[ImGuiCol_TitleBg] = fr;
		colors[ImGuiCol_TitleBgActive] = fr;
		colors[ImGuiCol_TitleBgCollapsed] = fr;

		//==========================================================
		// Menu
		//==========================================================

		colors[ImGuiCol_MenuBarBg] = C(26, 32, 43);

		//==========================================================
		// Scrollbars
		//==========================================================

		colors[ImGuiCol_ScrollbarBg] = C(28, 35, 47);
		colors[ImGuiCol_ScrollbarGrab] = C(82, 90, 110);
		colors[ImGuiCol_ScrollbarGrabHovered] = C(120, 128, 150);
		colors[ImGuiCol_ScrollbarGrabActive] = C(150, 160, 185);

		//==========================================================
		// Checkmarks
		//==========================================================

		colors[ImGuiCol_CheckMark] = C(165, 120, 255);

		//==========================================================
		// Sliders
		//==========================================================

		colors[ImGuiCol_SliderGrab] = C(138, 94, 255);
		colors[ImGuiCol_SliderGrabActive] = C(173, 134, 255);

		//==========================================================
		// Buttons
		//==========================================================

		colors[ImGuiCol_Button] = C(52, 60, 78, 0);
		colors[ImGuiCol_ButtonHovered] = C(110, 82, 210, 40);
		colors[ImGuiCol_ButtonActive] = C(138, 94, 255, 0);

		//==========================================================
		// Headers
		//==========================================================

		colors[ImGuiCol_Header] = C(95, 70, 175);
		colors[ImGuiCol_HeaderHovered] = C(125, 95, 225);
		colors[ImGuiCol_HeaderActive] = C(155, 125, 255);

		//==========================================================
		// Tabs
		//==========================================================

		colors[ImGuiCol_Tab] = fr;
		colors[ImGuiCol_TabHovered] = fr;
		colors[ImGuiCol_TabActive] = fr;
		colors[ImGuiCol_TabSelected] = fr;
		colors[ImGuiCol_TabDimmed] = fr;
		colors[ImGuiCol_TabDimmedSelected] = fr;
		colors[ImGuiCol_TabDimmedSelectedOverline] = C(0, 0, 0, 0);
		colors[ImGuiCol_TabSelectedOverline] = C(0, 0, 0, 0);

		//==========================================================
		// Resize grips
		//==========================================================

		colors[ImGuiCol_ResizeGrip] = C(120, 90, 220, 120);
		colors[ImGuiCol_ResizeGripHovered] = C(145, 110, 240);
		colors[ImGuiCol_ResizeGripActive] = C(170, 135, 255);

		//==========================================================
		// Separators
		//==========================================================

		colors[ImGuiCol_Separator] = C(60, 70, 90);
		colors[ImGuiCol_SeparatorHovered] = C(120, 95, 225);
		colors[ImGuiCol_SeparatorActive] = C(145, 115, 245);

		//==========================================================
		// Tables
		//==========================================================

		colors[ImGuiCol_TableHeaderBg] = C(34, 41, 55);
		colors[ImGuiCol_TableBorderStrong] = C(0, 0, 0, 0);
		colors[ImGuiCol_TableBorderLight] = C(0, 0, 0, 0);

		colors[ImGuiCol_TableRowBg] = C(27, 37, 50);
		colors[ImGuiCol_TableRowBgAlt] = C(25, 35, 47);

		//==========================================================
		// Selection
		//==========================================================

		colors[ImGuiCol_TextSelectedBg] = C(120, 95, 225, 120);

		//==========================================================
		// Docking
		//==========================================================

		colors[ImGuiCol_DockingPreview] = C(138, 94, 255, 180);
		colors[ImGuiCol_DockingEmptyBg] = fr;

		//==========================================================
		// Navigation
		//==========================================================

		colors[ImGuiCol_NavCursor] = C(170, 135, 255);
		colors[ImGuiCol_NavWindowingHighlight] = C(170, 135, 255);

		//----------------------------------------------------------
		// Make everything slightly larger
		//----------------------------------------------------------

		style.ScaleAllSizes(1.05f);

		//----------------------------------------------------------
		// Extra colors
		//----------------------------------------------------------

		ImVec4* extraColors = imgui::ExtraColors;
		extraColors[imgui::ImGuiExtraCol_Accent] = C(70, 50, 115, 255);
		extraColors[imgui::ImGuiExtraCol_AccentHovered] = C(90, 90, 155, 255);
		extraColors[imgui::ImGuiExtraCol_AccentActive] = C(65, 47, 107, 255);
		extraColors[imgui::ImGuiExtraCol_TabInactive] = C(20, 29, 40, 255);

		ImPlotStyle& implotStyle = ImPlot::GetStyle();
		implotStyle.LabelPadding = ImVec2(12, 12);

		ImVec4* implotColors = implotStyle.Colors;
		ImPlot::GetStyle().Colors[ImPlotCol_Line] = C(111, 84, 164, 255);

		// Override colors.
		colors[ImGuiCol_PlotHistogram] = extraColors[imgui::ImGuiExtraCol_Accent];
	}
}