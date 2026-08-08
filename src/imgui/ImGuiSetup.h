#pragma once

#include <imgui/imgui.h>

namespace humongousexplorer::imgui
{
	void Initialize();
	void Render();
	void UpdateMouseCursor();

	ImFont* GetDefaultFont();
	ImFont* GetBoldFont();
}