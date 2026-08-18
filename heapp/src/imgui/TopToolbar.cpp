#include "TopToolbar.h"

#include <imgui.h>

#include "dx11/SVGTextureCache.h"
#include "imgui/ImGuiSetup.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void TopToolbar::Render()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(
			ImVec2(viewport->Pos.x, viewport->Pos.y + m_fSize)
		);
		ImGui::SetNextWindowSize(
			ImVec2(viewport->Size.x, m_fSize)
		);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImVec2 padding = ImGui::GetStyle().WindowPadding;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		if (ImGui::Begin("TopMenuBar", nullptr, flags))
		{
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
	}

	//---------------------------------------------------------------------
	float TopToolbar::GetSize() const
	{
		return m_fSize;
	}
}