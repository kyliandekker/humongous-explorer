#include "Theme.h"

#include <imgui/imgui.h>
#include <imgui/implot.h>

#include "imgui/ImGuiSetup.h"

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
        style.FramePadding = ImVec2(8, 8);
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

        auto RGB = [](int r, int g, int b, int a = 255)
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

        colors[ImGuiCol_Text] = RGB(209, 211, 212);
        colors[ImGuiCol_TextDisabled] = RGB(135, 145, 165);

        //==========================================================
        // Frames
        //==========================================================

        ImColor fr = RGB(24, 35, 47);
        colors[ImGuiCol_FrameBg] = RGB(20, 30, 41);
        colors[ImGuiCol_FrameBgHovered] = RGB(55, 67, 90);
        colors[ImGuiCol_FrameBgActive] = RGB(80, 60, 135);

        //==========================================================
        // Windows
        //==========================================================

        colors[ImGuiCol_WindowBg] = fr;
        colors[ImGuiCol_ChildBg] = RGB(28, 35, 47);
        colors[ImGuiCol_PopupBg] = RGB(33, 40, 55);

        //==========================================================
        // Borders
        //==========================================================

        colors[ImGuiCol_Border] = RGB(44, 54, 69);
        colors[ImGuiCol_BorderShadow] = RGB(0, 0, 0, 0);

        //==========================================================
        // Title
        //==========================================================

        colors[ImGuiCol_TitleBg] = fr;
        colors[ImGuiCol_TitleBgActive] = fr;
        colors[ImGuiCol_TitleBgCollapsed] = fr;

        //==========================================================
        // Menu
        //==========================================================

        colors[ImGuiCol_MenuBarBg] = RGB(26, 32, 43);

        //==========================================================
        // Scrollbars
        //==========================================================

        colors[ImGuiCol_ScrollbarBg] = RGB(28, 35, 47);
        colors[ImGuiCol_ScrollbarGrab] = RGB(82, 90, 110);
        colors[ImGuiCol_ScrollbarGrabHovered] = RGB(120, 128, 150);
        colors[ImGuiCol_ScrollbarGrabActive] = RGB(150, 160, 185);

        //==========================================================
        // Checkmarks
        //==========================================================

        colors[ImGuiCol_CheckMark] = RGB(165, 120, 255);

        //==========================================================
        // Sliders
        //==========================================================

        colors[ImGuiCol_SliderGrab] = RGB(138, 94, 255);
        colors[ImGuiCol_SliderGrabActive] = RGB(173, 134, 255);

        //==========================================================
        // Buttons
        //==========================================================

        colors[ImGuiCol_Button] = RGB(52, 60, 78, 0);
        colors[ImGuiCol_ButtonHovered] = RGB(110, 82, 210, 40);
        colors[ImGuiCol_ButtonActive] = RGB(138, 94, 255, 0);

        //==========================================================
        // Headers
        //==========================================================

        colors[ImGuiCol_Header] = RGB(95, 70, 175);
        colors[ImGuiCol_HeaderHovered] = RGB(125, 95, 225);
        colors[ImGuiCol_HeaderActive] = RGB(155, 125, 255);

        //==========================================================
        // Tabs
        //==========================================================

        colors[ImGuiCol_Tab] = fr;
        colors[ImGuiCol_TabHovered] = fr;
        colors[ImGuiCol_TabActive] = fr;
        colors[ImGuiCol_TabSelected] = fr;
        colors[ImGuiCol_TabDimmed] = fr;
        colors[ImGuiCol_TabDimmedSelected] = fr;
        colors[ImGuiCol_TabDimmedSelectedOverline] = RGB(0, 0, 0, 0);
        colors[ImGuiCol_TabSelectedOverline] = RGB(0, 0, 0, 0);

        //==========================================================
        // Resize grips
        //==========================================================

        colors[ImGuiCol_ResizeGrip] = RGB(120, 90, 220, 120);
        colors[ImGuiCol_ResizeGripHovered] = RGB(145, 110, 240);
        colors[ImGuiCol_ResizeGripActive] = RGB(170, 135, 255);

        //==========================================================
        // Separators
        //==========================================================

        colors[ImGuiCol_Separator] = RGB(60, 70, 90);
        colors[ImGuiCol_SeparatorHovered] = RGB(120, 95, 225);
        colors[ImGuiCol_SeparatorActive] = RGB(145, 115, 245);

        //==========================================================
        // Tables
        //==========================================================

        colors[ImGuiCol_TableHeaderBg] = RGB(34, 41, 55);
        colors[ImGuiCol_TableBorderStrong] = RGB(0, 0, 0, 0);
        colors[ImGuiCol_TableBorderLight] = RGB(0, 0, 0, 0);

        colors[ImGuiCol_TableRowBg] = RGB(27, 37, 50);
        colors[ImGuiCol_TableRowBgAlt] = RGB(25, 35, 47);

        //==========================================================
        // Selection
        //==========================================================

        colors[ImGuiCol_TextSelectedBg] = RGB(120, 95, 225, 120);

        //==========================================================
        // Docking
        //==========================================================

        colors[ImGuiCol_DockingPreview] = RGB(138, 94, 255, 180);
        colors[ImGuiCol_DockingEmptyBg] = fr;

        //==========================================================
        // Navigation
        //==========================================================

        colors[ImGuiCol_NavCursor] = RGB(170, 135, 255);
        colors[ImGuiCol_NavWindowingHighlight] = RGB(170, 135, 255);

        //----------------------------------------------------------
        // Make everything slightly larger
        //----------------------------------------------------------

        style.ScaleAllSizes(1.05f);

        //----------------------------------------------------------
        // Extra colors
        //----------------------------------------------------------

        ImVec4* extraColors = imgui::ExtraColors;
        extraColors[imgui::ImGuiExtraCol_Accent] = RGB(70, 50, 115, 255);
        extraColors[imgui::ImGuiExtraCol_AccentHovered] = RGB(90, 90, 155, 255);
        extraColors[imgui::ImGuiExtraCol_AccentActive] = RGB(65, 47, 107, 255);
        extraColors[imgui::ImGuiExtraCol_TabInactive] = RGB(20, 29, 40, 255);

        ImPlotStyle& implotStyle = ImPlot::GetStyle();
        implotStyle.LabelPadding = ImVec2(12, 12);

        ImVec4* implotColors = implotStyle.Colors;
        ImPlot::GetStyle().Colors[ImPlotCol_Line] = RGB(111, 84, 164, 255);

        // Override colors.
        colors[ImGuiCol_PlotHistogram] = extraColors[imgui::ImGuiExtraCol_Accent];
    }
}