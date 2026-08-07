#include "EditorWindowDock.h"

#include <imgui.h>
#include <imgui/Helpers.h>

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void EditorWindowDock::Render()
	{
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu(FormatId(std::string(" File"), MENU_ID, "FILE", "DOCK").c_str()))
		{
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

		MainWindowDock::Render();
	}
}