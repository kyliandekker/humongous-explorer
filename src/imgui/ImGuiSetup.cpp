#include "imgui/ImGuiSetup.h"

#include <imgui/imgui_internal.h>
#include <imgui/font_arial.h>
#include <win32/WINPCH.h>

#include "imgui/Theme.h"
#include "imgui/windows/EditorWindowDock.h"
#include "imgui/windows/RoomContentWindow.h"
#include "imgui/windows/ArchiveContentsWindow.h"
#include "editor/Workspace.h"

namespace humongousexplorer::imgui
{
	EditorWindowDock m_MainDock;
	RoomContentWindow m_RoomContentWindow;
	ArchiveContentsWindow m_ArchiveContentsWindow;

	bool m_bDockLayoutSetup = false;
	std::string m_sIniPath;

	//---------------------------------------------------------------------
	void SetupDockLayout()
	{
		ImGuiID dockspaceId = ImGui::GetID("DockSpace");

		ImGui::DockBuilderRemoveNode(dockspaceId);
		ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		float dockHeight = viewport->WorkSize.y - 42.0f - 28.0f - 24.0f;
		ImGui::DockBuilderSetNodeSize(dockspaceId, ImVec2(viewport->WorkSize.x, dockHeight));
	}

	//---------------------------------------------------------------------
	ImFont* m_pDefaultFont = nullptr;
	ImFont* m_pBoldFont = nullptr;
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

		io.Fonts->Build();

		m_sIniPath = GetWorkspace().GetAppDataPath() + "/imgui.ini";
		io.IniFilename = m_sIniPath.c_str();

		ApplyTheme();
		m_MainDock.Initialize();
		m_ArchiveContentsWindow.Initialize();
		m_RoomContentWindow.Initialize();
	}

	//---------------------------------------------------------------------
	void Render()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		m_MainDock.Render();
		m_ArchiveContentsWindow.Render();
		m_RoomContentWindow.Render();

		if (!m_bDockLayoutSetup)
		{
			SetupDockLayout();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

		ImGui::PopStyleVar();
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