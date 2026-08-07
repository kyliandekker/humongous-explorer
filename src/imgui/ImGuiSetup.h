#pragma once

#include <imgui/imgui.h>

namespace humongousexplorer::imgui
{
	void Initialize();
	void Render();

	ImFont* GetDefaultFont();
	ImFont* GetBoldFont();
}