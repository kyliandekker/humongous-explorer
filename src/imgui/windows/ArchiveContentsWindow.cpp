#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "dx11/SVGTextureCache.h"

#include "imgui/ImGuiSetup.h"
#include "imgui/FileEntryView.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	ArchiveContentsWindow::ArchiveContentsWindow()
		: BaseWindow(ImGuiWindowFlags_NoCollapse, "ARCHIVE CONTENTS", "ArchiveContentsWindow")
	{
	}

	//---------------------------------------------------------------------
	bool imgui::ArchiveContentsWindow::Initialize()
	{
		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::Update()
	{
	}
}