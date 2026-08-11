#include "imgui/ImGuiSetup.h"

#include <imgui/imgui_internal.h>
#include <imgui/font_arial.h>
#include <imgui/icon.h>
#include <win32/WINPCH.h>

#include "imgui/Theme.h"
#include "imgui/windows/EditorWindowDock.h"
#include "imgui/windows/ArchiveContentsWindow.h"
#include "imgui/windows/RoomContentWindow.h"
#include "imgui/windows/PreviewWindow.h"
#include "imgui/windows/InfoPanelWindow.h"
#include "imgui/BottomToolbar.h"
#include "imgui/TopToolbar.h"
#include "editor/Workspace.h"

namespace humongousexplorer::imgui
{
	EditorWindowDock m_MainDock;
	RoomContentWindow m_RoomContentWindow;
	ArchiveContentsWindow m_ArchiveContentsWindow;
	PreviewWindow m_PreviewWindow;
	InfoPanelWindow m_InfoPanelWindow;
	BottomToolbar m_BottomToolbar;
	TopToolbar m_TopToolbar;

	std::string m_sIniPath;

	//---------------------------------------------------------------------
	ImFont* m_pDefaultFont = nullptr;
	ImFont* m_pBoldFont = nullptr;
	ImFont* m_pIconFont = nullptr;
	void Initialize()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		(void)io;

		ImFontConfig font_config_bold;
		font_config_bold.FontDataOwnedByAtlas = false;
		m_pBoldFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBold, sizeof(font::arialBold), 15, &font_config_bold);

		ImFontConfig font_config_default;
		font_config_default.FontDataOwnedByAtlas = false;
		m_pDefaultFont = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), 15, &font_config_default);

		constexpr ImWchar icons_ranges_b[] = { icon::FONT_START, icon::FONT_END, 0 };
		ImFontConfig icons_config_m;
		icons_config_m.MergeMode = true;
		icons_config_m.PixelSnapH = true;
		icons_config_m.FontDataOwnedByAtlas = false;
		m_pIconFont = io.Fonts->AddFontFromMemoryTTF(&icon::ICON, sizeof(icon::ICON), 15, &icons_config_m, icons_ranges_b);

		io.Fonts->Build();

		m_sIniPath = GetWorkspace().GetAppDataPath() + "/imgui.ini";
		io.IniFilename = m_sIniPath.c_str();

		ApplyTheme();
	}

	//---------------------------------------------------------------------
	void Render()
	{
		m_MainDock.Initialize();
		m_ArchiveContentsWindow.Initialize();
		m_RoomContentWindow.Initialize();
		m_PreviewWindow.Initialize();
		m_InfoPanelWindow.Initialize();

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		m_TopToolbar.Render();
		m_BottomToolbar.Render();

		ImVec2 padding = ImGui::GetStyle().WindowPadding;
		ImGui::SetNextWindowPos(
			ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + m_BottomToolbar.GetSize() + padding.y)
		);
		ImGui::SetNextWindowSize(
			ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - (m_BottomToolbar.GetSize() + m_TopToolbar.GetSize() + +padding.y))
		);

		m_MainDock.Render();
		m_ArchiveContentsWindow.Render();
		m_RoomContentWindow.Render();
		m_PreviewWindow.Render();
		m_InfoPanelWindow.Render();
	}

	void UpdateMouseCursor()
	{
		if (ImGui::IsAnyItemHovered() && ImGui::GetMouseCursor() == ImGuiMouseCursor_Arrow)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		LPTSTR win32_cursor = IDC_ARROW;
		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		switch (imgui_cursor)
		{
			case ImGuiMouseCursor_TextInput:
			{
				win32_cursor = IDC_IBEAM;
				break;
			}
			case ImGuiMouseCursor_ResizeAll:
			{
				win32_cursor = IDC_SIZEALL;
				break;
			}
			case ImGuiMouseCursor_ResizeNS:
			{
				win32_cursor = IDC_SIZENS;
				break;
			}
			case ImGuiMouseCursor_ResizeEW:
			{
				win32_cursor = IDC_SIZEWE;
				break;
			}
			case ImGuiMouseCursor_ResizeNESW:
			{
				win32_cursor = IDC_SIZENESW;
				break;
			}
			case ImGuiMouseCursor_ResizeNWSE:
			{
				win32_cursor = IDC_SIZENWSE;
				break;
			}
			case ImGuiMouseCursor_Hand:
			{
				win32_cursor = IDC_HAND;
				break;
			}
			case ImGuiMouseCursor_NotAllowed:
			{
				win32_cursor = IDC_NO;
				break;
			}
			default:
			{
				win32_cursor = IDC_ARROW;
				break;
			}
		}

		// Set the system cursor using Win32 API
		::SetCursor(LoadCursor(NULL, win32_cursor));
	}

	//---------------------------------------------------------------------
	static std::string s_sDroppedFile;
	static ImVec2 s_vDroppedFilePos = {};

	void SetDroppedFile(const std::string& a_sPath, ImVec2 a_vDropPos)
	{
		s_sDroppedFile = a_sPath;
		s_vDroppedFilePos = a_vDropPos;
	}

	std::string ConsumeDroppedFile()
	{
		std::string path = std::move(s_sDroppedFile);
		s_sDroppedFile.clear();
		return path;
	}

	ImVec2 GetDroppedFilePosition()
	{
		ImVec2 pos = s_vDroppedFilePos;
		s_vDroppedFilePos = {};
		return pos;
	}

	//---------------------------------------------------------------------
	ImFont* GetDefaultFont()
	{
		return m_pDefaultFont;
	}

	//---------------------------------------------------------------------
	ImFont* GetBoldFont()
	{
		return m_pBoldFont;
	}
}