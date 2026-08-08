#include "./SearchBar.h"

#include <imgui.h>

#include "dx11/SVGTextureCache.h"

#include "imgui/Helpers.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	bool SearchBar::Render()
	{
		bool changed = false;

		float windowWidth = ImGui::GetContentRegionAvail().x;
		float iconSize = ImGui::GetFontSize();
		float iconPadding = 8.0f;
		float textOffset = iconSize + iconPadding * 2;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(textOffset, 6.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(35, 42, 58, 255));

		ImGui::PushItemWidth(windowWidth - 16.0f);
		changed = ImGui::InputTextWithHint(FormatId("", SEARCHBAR_ID, m_sName).c_str(), m_sHint.c_str(), m_aBuffer, sizeof(m_aBuffer));
		ImGui::PopItemWidth();

		ID3D11ShaderResourceView* pTex = dx11::SVGTextureCache::Get(m_sIconPath.c_str());
		if (pTex)
		{
			ImVec2 pos = ImGui::GetItemRectMin();
			float iconY = pos.y + (ImGui::GetItemRectSize().y - iconSize) * 0.5f;
			ImGui::GetWindowDrawList()->AddImage(
				(ImTextureID)pTex,
				ImVec2(pos.x + iconPadding, iconY),
				ImVec2(pos.x + iconPadding + iconSize, iconY + iconSize)
			);
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::Spacing();

		return changed;
	}
}