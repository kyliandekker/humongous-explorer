#pragma once

#include <imgui/imgui.h>

namespace humongousexplorer::imgui
{
	enum ImGuiExtraCol_
	{
		ImGuiExtraCol_Accent, 
		ImGuiExtraCol_AccentHovered, 
		ImGuiExtraCol_TabInactive, 
		ImGuiExtraCol_COUNT
	};
	inline ImVec4 ExtraColors[ImGuiExtraCol_COUNT];

	void Initialize();
	void Render();
	void UpdateMouseCursor();

	ImFont* GetDefaultFont();
	ImFont* GetBoldFont();
}