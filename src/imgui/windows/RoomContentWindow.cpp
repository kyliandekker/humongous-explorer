#include "./RoomContentWindow.h"

#include <imgui/imgui.h>
#include <string>
#include <vector>
#include <algorithm>

#include "imgui/ImGuiSetup.h"
#include "imgui/views/SearchBar.h"
#include "resources/ResourceType.h"
#include "dx11/SVGTextureCache.h"
#include "utils/string_extensions.h"
#include "imgui/Helpers.h"

namespace humongousexplorer::imgui
{
	struct ResourceEntry
	{
		std::string sName;
		resources::ResourceType eType;
		std::string sSize;
		std::string sDimensions;
		size_t m_iOrder;
	};

	static std::vector<ResourceEntry> s_aResources =
	{
		{ "HELOGO_BACKGROUND", resources::ResourceType::RoomBackground, "245 KB", "320x200" },
		{ "HELOGO_IMAGE", resources::ResourceType::RoomImage, "128 KB", "320x200" },
		{ "HELOGO_SCRIPT", resources::ResourceType::LocalScript, "12 KB", "-" },
		{ "SHARED_SFX_HEINTRO", resources::ResourceType::SFX, "12 KB", "-" },
	};

	//---------------------------------------------------------------------
	// RoomContentWindow
	//---------------------------------------------------------------------
	RoomContentWindow::RoomContentWindow()
		: HEBaseWindow(ImGuiWindowFlags_NoCollapse, "ROOM CONTENT", "RoomContentWindow"),
		m_SearchBar("RoomSearchbar", "Search resources...")
	{
	}

	//---------------------------------------------------------------------
	bool RoomContentWindow::Initialize()
	{
		size_t i = 0;
		for (auto& entry : s_aResources)
		{
			entry.m_iOrder = i;
			i++;
		}
		return HEBaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	int compare_size_t(size_t a, size_t b) {
		if (a < b) return -1;
		if (a > b) return 1;
		return 0;
	}

	//---------------------------------------------------------------------
	void RoomContentWindow::Update()
	{
		m_SearchBar.Render();

		std::string filter = string_extensions::StringToLower(m_SearchBar.GetText());

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));

		if (ImGui::BeginTable("ResourceTable", 7,
			ImGuiTableFlags_Borders |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_Resizable |
			ImGuiTableFlags_ScrollY |
			ImGuiTableFlags_Sortable |
			ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 0);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() + 8.0f);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 3.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableSetupColumn("Dimensions", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableHeadersRow();

			ImVec2 avail = ImGui::GetContentRegionAvail();
			float iconSize = ImGui::GetFontSize();
			float rowHeight = ImGui::GetFrameHeight();

			for (size_t i = 0; i < s_aResources.size(); i++)
			{
				const ResourceEntry& entry = s_aResources[i];

				if (!filter.empty())
				{
					std::string lowerName = string_extensions::StringToLower(entry.sName);
					if (lowerName.find(filter) == std::string::npos)
					{
						continue;
					}
				}

				ImGui::TableNextRow();

				ImGui::TableNextColumn();

				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));          // disables background
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));   // disables hover highlight
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));    // disables pressed highlight

				bool isSelected = (m_iSelectedRow == static_cast<int>(i));

				// This spans the entire row internally.
				if (ImGui::Selectable(
					FormatId("", SELECTABLE_ID, m_sName, "ROW", std::to_string(i)).c_str(),
					isSelected,
					ImGuiSelectableFlags_SpanAllColumns,
					ImVec2(0.0f, 0.0f)
				))
				{
					m_iSelectedRow = static_cast<int>(i);
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0,
						ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_AccentHovered]));
				}
				if (isSelected)
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0,
						ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_Accent]));
				}

				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();

				ImGui::TableNextColumn();
				{
					ID3D11ShaderResourceView* pTex = dx11::SVGTextureCache::Get(resources::GetIconFromResourceType(entry.eType).c_str());
					if (pTex)
					{
						ImVec2 pos = ImGui::GetCursorScreenPos();
						ImGui::GetWindowDrawList()->AddImage(
							(ImTextureID)pTex,
							ImVec2(pos.x, pos.y + (rowHeight - iconSize) * 0.5f),
							ImVec2(pos.x + iconSize, pos.y + (rowHeight + iconSize) * 0.5f)
						);
					}
				}

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(entry.sName.c_str());

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(resources::GetNameFromResourceType(entry.eType).c_str());

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(entry.sSize.c_str());

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(entry.sDimensions.c_str());
			}

			if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs())
			{
				if (sortSpecs->SpecsDirty && sortSpecs->SpecsCount > 0)
				{
					const ImGuiTableColumnSortSpecs& spec = sortSpecs->Specs[0];
					m_iSortColumn = spec.ColumnIndex;
					m_bSortAscending = spec.SortDirection == ImGuiSortDirection_Ascending;
					sortSpecs->SpecsDirty = false;

					std::sort(s_aResources.begin(), s_aResources.end(), [this](const ResourceEntry& a, const ResourceEntry& b)
					{
						int cmp = 0;
						switch (m_iSortColumn)
						{
							case 0:
							case 1:
							{
								cmp = compare_size_t(a.m_iOrder, b.m_iOrder);
								break;
							}
							case 2:
							{
								cmp = a.sName.compare(b.sName); 
								break;
							}
							case 3:
							{
								cmp = resources::GetNameFromResourceType(a.eType).compare(resources::GetNameFromResourceType(b.eType)); 
								break;
							}
							case 4:
							{
								cmp = a.sSize.compare(b.sSize); 
								break;
							}
							case 5:
							{
								cmp = a.sDimensions.compare(b.sDimensions); 
								break;
							}
						}
						return m_bSortAscending ? cmp < 0 : cmp > 0;
					});
				}
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleVar();
	}
}