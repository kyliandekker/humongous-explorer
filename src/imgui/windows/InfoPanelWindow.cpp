#include "./InfoPanelWindow.h"

#include <imgui/imgui.h>

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// InfoPanelWindow
	//---------------------------------------------------------------------
	InfoPanelWindow::InfoPanelWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "INFO", "InfoPanelWindow")
	{
	}

	//---------------------------------------------------------------------
	bool imgui::InfoPanelWindow::Initialize()
	{
		return HEBaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void InfoPanelWindow::Update()
	{
	}
}