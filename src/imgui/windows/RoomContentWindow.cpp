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

namespace humongousexplorer::imgui
{
	struct ResourceEntry
	{
		std::string sName;
		resources::ResourceType eType;
		std::string sSize;
		std::string sDimensions;
		std::string sFormat;
	};

	static std::vector<ResourceEntry> s_aResources =
	{
		{ "HELOGO_BACKGROUND", resources::ResourceType::RoomBackground, "245 KB", "320x200", "WAL" }, 
		{ "HELOGO_IMAGE", resources::ResourceType::RoomImage, "128 KB", "320x200", "BMP" }, 
		{ "HELOGO_SCRIPT", resources::ResourceType::LocalScript, "12 KB", "-", "SCRP" }, 
		{ "INTERFACE_IMAGE", resources::ResourceType::RoomImage, "96 KB", "640x480", "BMP" }, 
		{ "INTERFACE_GLOBALSCRIPT", resources::ResourceType::GlobalScript, "8 KB", "-", "SCRP" }, 
		{ "SAVELOAD_IMAGE", resources::ResourceType::RoomImage, "64 KB", "320x200", "BMP" }, 
		{ "SAVELOAD_VERBSCRIPT", resources::ResourceType::VerbScript, "16 KB", "-", "SCRP" }, 
		{ "SPYWATCH_IMAGE", resources::ResourceType::RoomImage, "112 KB", "640x480", "BMP" }, 
		{ "SPYWATCH_LOCALSCRIPT", resources::ResourceType::LocalScript, "10 KB", "-", "SCRP" }, 
		{ "MOBCOM_IMAGE", resources::ResourceType::RoomImage, "84 KB", "640x480", "BMP" }, 
		{ "MOBCOM_LOCALSCRIPT", resources::ResourceType::LocalScript, "14 KB", "-", "SCRP" }, 
		{ "SHARED_TALKIE_VOICE1", resources::ResourceType::Talkie, "1.2 MB", "-", "SOU" }, 
		{ "SHARED_TALKIE_VOICE2", resources::ResourceType::Talkie, "980 KB", "-", "SOU" }, 
		{ "SHARED_SONG_BACKGROUND", resources::ResourceType::Song, "3.4 MB", "-", "SOU" }, 
		{ "SHARED_SFX_DOOR", resources::ResourceType::SFX, "24 KB", "-", "SOU" }, 
		{ "SHARED_SFX_FOOTSTEP", resources::ResourceType::SFX, "12 KB", "-", "SOU" },
	};

	//---------------------------------------------------------------------
	// RoomContentWindow
	//---------------------------------------------------------------------
	RoomContentWindow::RoomContentWindow()
		: BaseWindow(ImGuiWindowFlags_NoCollapse, "ROOM CONTENT", "RoomContentWindow"),
		m_SearchBar("RoomSearchbar", "Search resources...")
	{
	}

	//---------------------------------------------------------------------
	bool RoomContentWindow::Initialize()
	{
		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void RoomContentWindow::Update()
	{
		m_SearchBar.Render();

		std::string filter = string_extensions::StringToLower(m_SearchBar.GetText());

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));

		if (ImGui::BeginTable("ResourceTable", 6,
			ImGuiTableFlags_Borders |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_Resizable |
			ImGuiTableFlags_ScrollY |
			ImGuiTableFlags_Sortable |
			ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() + 8.0f);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 3.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch, 2.0f);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableSetupColumn("Dimensions", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableSetupColumn("Format", ImGuiTableColumnFlags_WidthStretch, 1.0f);
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

				bool isSelected = (m_iSelectedRow == static_cast<int>(i));
				if (isSelected)
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(80, 60, 135, 255));
				}

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
					ImGui::Dummy(ImVec2(iconSize, rowHeight));
					if (ImGui::IsItemClicked())
					{
						m_iSelectedRow = static_cast<int>(i);
					}
				}

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(entry.sName.c_str());
				if (ImGui::IsItemClicked())
				{
					m_iSelectedRow = static_cast<int>(i);
				}

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(resources::GetNameFromResourceType(entry.eType).c_str());
				if (ImGui::IsItemClicked())
				{
					m_iSelectedRow = static_cast<int>(i);
				}

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(entry.sSize.c_str());
				if (ImGui::IsItemClicked())
				{
					m_iSelectedRow = static_cast<int>(i);
				}

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(entry.sDimensions.c_str());
				if (ImGui::IsItemClicked())
				{
					m_iSelectedRow = static_cast<int>(i);
				}

				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(entry.sFormat.c_str());
				if (ImGui::IsItemClicked())
				{
					m_iSelectedRow = static_cast<int>(i);
				}
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
							case 1: cmp = a.sName.compare(b.sName); break;
							case 2: cmp = resources::GetNameFromResourceType(a.eType).compare(resources::GetNameFromResourceType(b.eType)); break;
							case 3: cmp = a.sSize.compare(b.sSize); break;
							case 4: cmp = a.sDimensions.compare(b.sDimensions); break;
							case 5: cmp = a.sFormat.compare(b.sFormat); break;
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