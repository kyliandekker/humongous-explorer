#include "./FileEntryView.h"

#include <imgui.h>

#include "dx11/SVGTextureCache.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void TextRowEntry::Render(const ImVec2& a_vPos)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddText(a_vPos, m_Color, m_sText.c_str());
	}

	//---------------------------------------------------------------------
	ImVec2 TextRowEntry::GetSize()
	{
		return ImGui::CalcTextSize(m_sText.c_str());
	}

	//---------------------------------------------------------------------
	void IconRowEntry::Render(const ImVec2& a_vPos)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Icon
		ID3D11ShaderResourceView* tex = dx11::SVGTextureCache::Get(m_sIconName);
		if (tex)
		{
			int nativeW = dx11::SVGTextureCache::GetWidth(m_sIconName);
			int nativeH = dx11::SVGTextureCache::GetHeight(m_sIconName);
			float scale = m_fSize / static_cast<float>((nativeW > nativeH) ? nativeW : nativeH);
			float drawW = nativeW * scale;
			float drawH = nativeH * scale;

			ImVec2 iconMin = ImVec2(a_vPos.x + 4.0f, a_vPos.y);
			ImVec2 iconMax = ImVec2(iconMin.x + drawW, iconMin.y + drawH);
			drawList->AddImage(
				static_cast<ImTextureID>(reinterpret_cast<intptr_t>(tex)),
				iconMin,
				iconMax,
				ImVec2(0, 0), ImVec2(1, 1),
				IM_COL32(255, 255, 255, 255));
		}
	}

	//---------------------------------------------------------------------
	ImVec2 IconRowEntry::GetSize()
	{
		float drawW = 0;
		float drawH = 0;
		ID3D11ShaderResourceView* tex = dx11::SVGTextureCache::Get(m_sIconName);
		if (tex)
		{
			int nativeW = dx11::SVGTextureCache::GetWidth(m_sIconName);
			int nativeH = dx11::SVGTextureCache::GetHeight(m_sIconName);
			float scale = m_fSize / static_cast<float>((nativeW > nativeH) ? nativeW : nativeH);
			drawW = nativeW * scale;
			drawH = nativeH * scale;
		}

		return {
			drawW, drawH
		};
	}

	//---------------------------------------------------------------------
	FileEntryInteractionType FileEntryView::Render(std::function<bool()> a_fnSelected)
	{
		FileEntryInteractionType interaction = FileEntryInteractionType::None;

		bool selected = a_fnSelected();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const float rowHeight = ImGui::GetFontSize() + 15;

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

		float previousOffset = 0;
		for (const std::unique_ptr<RowEntry>& row : m_aRows)
		{
			ImVec2 entryPos = ImVec2(previousOffset, rowMin.y);
			entryPos.y = entryPos.y + ((rowHeight - row->GetSize().y) * 0.5f);

			switch (row->m_RowInfo.m_RowInfoTextAlignment)
			{
				case RowInfoTextAlignment::Left:
				{
					entryPos.x = rowMin.x + previousOffset + row->m_RowInfo.m_fExtraOffset;
					break;
				}
				case RowInfoTextAlignment::Right:
				{
					entryPos.x = rowMax.x - (row->GetSize().x + row->m_RowInfo.m_fExtraOffset);
					break;
				}
			}
			previousOffset = entryPos.x + row->GetSize().x;

			row->Render(entryPos);
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