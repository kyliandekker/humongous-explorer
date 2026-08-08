#include "imgui/ImGuiSetup.h"

#include <imgui/imgui_internal.h>
#include <imgui/font_arial.h>

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

		ImFontConfig font_config_default;
		font_config_default.FontDataOwnedByAtlas = false;
		m_pDefaultFont = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), 15, &font_config_default);

		ImFontConfig font_config_bold;
		font_config_bold.FontDataOwnedByAtlas = false;
		m_pBoldFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBold, sizeof(font::arialBold), 15, &font_config_bold);

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