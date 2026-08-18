#pragma once

#include <imgui/imgui.h>
#include <string>

namespace humongousexplorer::imgui
{
	enum ImGuiExtraCol_
	{
		ImGuiExtraCol_Accent, 
		ImGuiExtraCol_AccentHovered, 
		ImGuiExtraCol_AccentActive,
		ImGuiExtraCol_TabInactive, 
		ImGuiExtraCol_COUNT
	};
	inline ImVec4 ExtraColors[ImGuiExtraCol_COUNT];

	void Initialize();
	void Render();
	void UpdateMouseCursor();

	void SetDroppedFile(const std::string& a_sPath, ImVec2 a_vDropPos);
	std::string ConsumeDroppedFile();
	ImVec2 GetDroppedFilePosition();

	ImFont* GetDefaultFont();
	ImFont* GetBoldFont();
}