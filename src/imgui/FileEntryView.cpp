#include "./FileEntryView.h"

#include <imgui.h>

#include "dx11/SVGTextureCache.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	FileEntryInteractionType FileEntryView::Render(std::function<bool()> a_fnSelected)
	{
		FileEntryInteractionType interaction = FileEntryInteractionType::None;

		bool selected = a_fnSelected();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const float iconSize = ImGui::GetFontSize() + 12.0f;
		const float rowHeight = iconSize + 4.0f;

		float windowWidth = ImGui::GetContentRegionAvail().x;
		ImVec2 windowPos = ImGui::GetCursorScreenPos();

		ImVec2 rowMin = ImVec2(windowPos.x, windowPos.y);
		ImVec2 rowMax = ImVec2(windowPos.x + windowWidth, rowMin.y + rowHeight);

		// Row background
		if (selected)
		{
			drawList->AddRectFilled(rowMin, rowMax, IM_COL32(80, 60, 135, 255), 4.0f);
		}
		else if (ImGui::IsMouseHoveringRect(rowMin, rowMax))
		{
			drawList->AddRectFilled(rowMin, rowMax, IM_COL32(55, 67, 90, 255), 4.0f);
		}

		// Click detection
		if (ImGui::IsMouseHoveringRect(rowMin, rowMax))
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				interaction = FileEntryInteractionType::LeftClicked;
			}
			else if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				interaction = FileEntryInteractionType::DoubleClicked;
			}
			else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				interaction = FileEntryInteractionType::RightClicked;
			}
		}

		// Icon
		ID3D11ShaderResourceView* tex = dx11::SVGTextureCache::Get(m_sIcon);
		if (tex)
		{
			int nativeW = dx11::SVGTextureCache::GetWidth(m_sIcon);
			int nativeH = dx11::SVGTextureCache::GetHeight(m_sIcon);
			float scale = iconSize / static_cast<float>((nativeW > nativeH) ? nativeW : nativeH);
			float drawW = nativeW * scale;
			float drawH = nativeH * scale;

			ImVec2 iconPos = ImVec2(rowMin.x + 4.0f, rowMin.y + (rowHeight - drawH) * 0.5f);
			drawList->AddImage(
				static_cast<ImTextureID>(reinterpret_cast<intptr_t>(tex)),
				iconPos,
				ImVec2(iconPos.x + drawW, iconPos.y + drawH),
				ImVec2(0, 0), ImVec2(1, 1),
				IM_COL32(255, 255, 255, 255));
		}

		for (const RowInfo& row : m_aRows)
		{
			// Label
			ImVec2 labelSize = ImGui::CalcTextSize(row.m_sName.c_str());
			ImVec2 labelPos = ImVec2(0, rowMin.y + ((rowHeight - labelSize.y) / 2));

			switch (row.m_RowInfoTextAlignment)
			{
				case RowInfoTextAlignment::Left:
				{
					labelPos.x = rowMin.x + iconSize + row.m_fExtraOffset;
					break;
				}
				case RowInfoTextAlignment::Right:
				{
					labelPos.x = rowMax.x - (labelSize.x + row.m_fExtraOffset);
					break;
				}
			}

			drawList->AddText(labelPos, row.m_Color, row.m_sName.c_str());
		}

		ImGui::Dummy({ windowWidth, rowHeight });

		return interaction;
	}

	//---------------------------------------------------------------------
	FileEntryInteractionType TreeFileEntryView::Render(std::function<bool()> a_fnSelected)
	{
		return FileEntryInteractionType::None;
	}
}