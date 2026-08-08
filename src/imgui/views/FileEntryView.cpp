#include "./FileEntryView.h"

#include <imgui.h>

#include "dx11/SVGTextureCache.h"

#include "utils/string_extensions.h"

#include "imgui/ImGuiSetup.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void TextRowEntry::Render(const ImVec2& a_vPos)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddText(a_vPos, m_Color, m_sText.c_str());
	}

	//---------------------------------------------------------------------
	ImVec2 TextRowEntry::GetSize() const
	{
		return ImGui::CalcTextSize(m_sText.c_str());
	}

	//---------------------------------------------------------------------
	bool TextRowEntry::Find(const std::string& a_sObjective) const
	{
		return string_extensions::StringToLower(m_sText).find(a_sObjective) != std::string::npos;
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
	ImVec2 IconRowEntry::GetSize() const
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
	bool IconRowEntry::Find(const std::string& a_sObjective) const
	{
		return false;
	}

	//---------------------------------------------------------------------
	void FileEntryView::Render(std::function<bool(FileEntryView* fileEntry)> a_fnSelected, std::function<void(FileEntryInteractionType, FileEntryView*)> a_fnOnInteraction, float a_fIndent)
	{
		FileEntryInteractionType interaction = FileEntryInteractionType::None;

		bool selected = a_fnSelected(this);

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const float rowHeight = ImGui::GetFontSize() + 15;

		float windowWidth = ImGui::GetContentRegionAvail().x;
		ImVec2 windowPos = ImGui::GetCursorScreenPos();

		ImVec2 rowMin = ImVec2(windowPos.x + a_fIndent, windowPos.y);
		ImVec2 rowMax = ImVec2(windowPos.x + windowWidth, rowMin.y + rowHeight);

		// Row background
		if (selected)
		{
			drawList->AddRectFilled(rowMin, rowMax, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_Accent]), 4.0f);
		}
		else if (ImGui::IsMouseHoveringRect(rowMin, rowMax))
		{
			drawList->AddRectFilled(rowMin, rowMax, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_AccentHovered]), 4.0f);
		}

		// Click detection
		if (ImGui::IsMouseHoveringRect(rowMin, rowMax))
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				interaction = FileEntryInteractionType::LeftClicked;
			}
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				interaction = FileEntryInteractionType::DoubleClicked;
			}
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
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
			previousOffset = entryPos.x - rowMin.x + row->GetSize().x;

			row->Render(entryPos);
		}

		if (interaction != FileEntryInteractionType::None)
		{
			a_fnOnInteraction(interaction, this);
		}

		ImGui::Dummy({ windowWidth, rowHeight });
	}

	//---------------------------------------------------------------------
	bool FileEntryView::Filter(const std::string& a_sObjective)
	{
		bool found = a_sObjective.empty();

		if (!found)
		{
			for (size_t i = 0; i < m_aRows.size(); i++)
			{
				if (m_aRows[i]->Find(a_sObjective))
				{
					found |= true;
				}
			}
		}
		m_bVisible = found;
		return found;
	}

	//---------------------------------------------------------------------
	void TreeFileEntryView::Render(std::function<bool(FileEntryView* fileEntry)> a_fnSelected, std::function<void(FileEntryInteractionType, FileEntryView*)> a_fnOnInteraction, float a_fIndent)
	{
		const float arrowSize = ImGui::GetFontSize() * 0.7f;
		const float indentStep = 20.0f;
		float arrowIndent = a_fIndent + arrowSize + 4.0f;

		// Draw expand/collapse arrow only if has children
		if (!m_aChildren.empty())
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			float rowHeight = ImGui::GetFontSize() + 15;

			ImVec2 center = ImVec2(pos.x + a_fIndent + arrowSize * 0.5f, pos.y + rowHeight * 0.5f);

			std::string iconName = m_bExpanded ?
				"../icons/icon_arrow_expanded.svg" : "../icons/icon_arrow.svg";
			ID3D11ShaderResourceView* tex = dx11::SVGTextureCache::Get(iconName);
			if (tex)
			{
				int nativeW = dx11::SVGTextureCache::GetWidth(iconName);
				int nativeH = dx11::SVGTextureCache::GetHeight(iconName);
				float scale = arrowSize / static_cast<float>((nativeW > nativeH) ? nativeW : nativeH);
				float drawW = nativeW * scale;
				float drawH = nativeH * scale;
				float halfX = drawW * 0.5f;
				float halfY = drawH * 0.5f;

				ImVec2 iconMin = ImVec2(center.x - halfX, pos.y + (rowHeight - drawH) * 0.5f);
				ImVec2 iconMax = ImVec2(center.x + halfX, pos.y + (rowHeight + drawH) * 0.5f);
				drawList->AddImage(
					static_cast<ImTextureID>(reinterpret_cast<intptr_t>(tex)),
					iconMin,
					iconMax,
					ImVec2(0, 0), ImVec2(1, 1),
					IM_COL32(255, 255, 255, 255));
			}

			// Click on arrow toggles expand
			ImVec2 arrowMin = ImVec2(pos.x + a_fIndent, pos.y);
			ImVec2 arrowMax = ImVec2(arrowMin.x + arrowSize, arrowMin.y + rowHeight);
			if (ImGui::IsMouseHoveringRect(arrowMin, arrowMax) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				m_bExpanded = !m_bExpanded;
			}
		}

		// Reuse base class rendering for own content
		FileEntryView::Render(a_fnSelected, a_fnOnInteraction, arrowIndent);

		// Render children if expanded
		if (m_bExpanded)
		{
			for (const std::unique_ptr<FileEntryView>& child : m_aChildren)
			{
				if (!child->m_bVisible)
				{
					continue;
				}

				child->Render(a_fnSelected, a_fnOnInteraction, arrowIndent + indentStep);
			}
		}
	}

	//---------------------------------------------------------------------
	bool TreeFileEntryView::Filter(const std::string& a_sObjective)
	{
		bool found = FileEntryView::Filter(a_sObjective);

		for (const std::unique_ptr<FileEntryView>& entry : m_aChildren)
		{
			found |= entry->Filter(a_sObjective);
		}

		m_bVisible = found;
		return found;
	}
}