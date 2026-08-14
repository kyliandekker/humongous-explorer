#include "EditorWindowDock.h"

#include <imgui.h>
#include <imgui/Helpers.h>

#include "file/file_abstractions.h"
#include "editor/Workspace.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void EditorWindowDock::Update()
	{
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu(FormatId(std::string(" File"), MENU_ID, "FILE", "DOCK").c_str()))
		{
			if (ImGui::MenuItem(FormatId(" Open Archive", BUTTON_ID, "OPEN_ARCHIVE").c_str()))
			{
				fs::path selected;
				std::vector<COMDLG_FILTERSPEC> filters = {
					{ L"HE Archive", L"*.(A);*.HE0;*.HE1;*.HE2;*.HE3;*.HE4;*.HE7;*.HE8" },
					{ L"All Files", L"*.*" }
				};
				if (file::PickFile(selected, filters))
				{
					GetWorkspace().LoadArchives(selected.string());
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(FormatId(std::string(" Edit"), MENU_ID, "EDIT", "DOCK").c_str()))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(FormatId(std::string(" View"), MENU_ID, "VIEW", "DOCK").c_str()))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(FormatId(std::string(" Tools"), MENU_ID, "TOOLS", "DOCK").c_str()))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(FormatId(std::string(" Help"), MENU_ID, "HELP", "DOCK").c_str()))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		MainWindowDock::Update();
	}
}