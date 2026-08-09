#include "./RoomContentWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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
	struct TabInfo
	{
		const char* sLabel;
		const char* sIconPath;
		std::vector<resources::ResourceType> m_aResourceTypes;
	};

	static const TabInfo s_aTabs[] =
	{
		{ "All Files",  "../icons/icon_all_files.svg", { resources::ResourceType::Unknown } },
		{ "Room Backgrounds",  "../icons/icon_background.svg", { resources::ResourceType::RoomBackground } },
		{ "Room Images",  "../icons/icon_image.svg", { resources::ResourceType::RoomImage, resources::ResourceType::RoomImageLayer } },
		{ "SFX",     "../icons/icon_sfx.svg", { resources::ResourceType::SFX } },
		{ "Talkies",     "../icons/icon_sfx.svg", { resources::ResourceType::Talkie } },
		{ "Songs",     "../icons/icon_sfx.svg", { resources::ResourceType::Song } },
		{ "Scripts", "../icons/icon_local_script.svg", { resources::ResourceType::GlobalScript, resources::ResourceType::LocalScript, resources::ResourceType::VerbScript } },
	};
	static const int s_iTabCount = sizeof(s_aTabs) / sizeof(s_aTabs[0]);

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
		{ "PUTT_IMAGE", resources::ResourceType::RoomImage, "45 KB", "72x72" },
		{ "PUP_IMAGE", resources::ResourceType::RoomImage, "22 KB", "64x64" },
		{ "TREE_IMAGE", resources::ResourceType::RoomImage, "68 KB", "128x256" },
		{ "HOUSE_IMAGE", resources::ResourceType::RoomImage, "12 KB", "320x200" },
		{ "MAILBOX_IMAGE", resources::ResourceType::RoomImage, "11 KB", "64x64" },
		{ "FLOWER_IMAGE", resources::ResourceType::RoomImage, "9 KB", "32x32" },
		{ "PATH_IMAGE", resources::ResourceType::RoomImage, "16 KB", "640x80" },
		{ "BUSH_IMAGE", resources::ResourceType::RoomImage, "15 KB", "96x64" },
		{ "CLOUD1_IMAGE", resources::ResourceType::RoomImage, "102 KB", "128x64" },
		{ "CLOUD2_IMAGE", resources::ResourceType::RoomImage, "99 KB", "160x80" },
		{ "FENCE_IMAGE", resources::ResourceType::RoomImage, "2 KB", "320x48" },
		{ "TALKIE_INTRO_VO", resources::ResourceType::Talkie, "1200 KB", "-" },
		{ "TALKIE_GREET_VO", resources::ResourceType::Talkie, "850 KB", "-" },
		{ "TALKIE_BYE_VO", resources::ResourceType::Talkie, "620 KB", "-" },
		{ "TALKIE_YES_VO", resources::ResourceType::Talkie, "410 KB", "-" },
		{ "TALKIE_NO_VO", resources::ResourceType::Talkie, "380 KB", "-" },
		{ "TALKIE_LOOK_VO", resources::ResourceType::Talkie, "540 KB", "-" },
		{ "SFX_DOOR_CLOSE", resources::ResourceType::SFX, "24 KB", "-" },
		{ "SFX_FOOTSTEP_1", resources::ResourceType::SFX, "12 KB", "-" },
		{ "SFX_FOOTSTEP_2", resources::ResourceType::SFX, "12 KB", "-" },
		{ "SFX_BIRD_CHIRP", resources::ResourceType::SFX, "18 KB", "-" },
		{ "SFX_WIND", resources::ResourceType::SFX, "45 KB", "-" },
		{ "SFX_CAR_HORN", resources::ResourceType::SFX, "8 KB", "-" },
		{ "SFX_PHONE_RING", resources::ResourceType::SFX, "32 KB", "-" },
		{ "SFX_WATER_DRIP", resources::ResourceType::SFX, "15 KB", "-" },
		{ "SFX_THUNDER", resources::ResourceType::SFX, "55 KB", "-" },
		{ "SFX_GLASS_BREAK", resources::ResourceType::SFX, "28 KB", "-" },
		{ "SFX_UI_CLICK", resources::ResourceType::SFX, "4 KB", "-" },
		{ "SFX_UI_HOVER", resources::ResourceType::SFX, "3 KB", "-" },
		{ "SFX_UI_BACK", resources::ResourceType::SFX, "5 KB", "-" },
		{ "SFX_COIN_DROP", resources::ResourceType::SFX, "11 KB", "-" },
		{ "SFX_PICKUP", resources::ResourceType::SFX, "9 KB", "-" },
		{ "SFX_USE_ITEM", resources::ResourceType::SFX, "14 KB", "-" },
		{ "SFX_GIVE_ITEM", resources::ResourceType::SFX, "13 KB", "-" },
		{ "SFX_SECRET_FOUND", resources::ResourceType::SFX, "22 KB", "-" },
		{ "SFX_DOOR_CREAK", resources::ResourceType::SFX, "19 KB", "-" },
		{ "SONG_TITLE", resources::ResourceType::Song, "3400 KB", "-" },
		{ "SONG_YARD_THEME", resources::ResourceType::Song, "2800 KB", "-" },
		{ "SONG_THEME", resources::ResourceType::Song, "2600 KB", "-" },
		{ "SONG_CHASE", resources::ResourceType::Song, "1900 KB", "-" },
		{ "SONG_ENDING", resources::ResourceType::Song, "4200 KB", "-" },
		{ "SONG PUZZLE", resources::ResourceType::Song, "1500 KB", "-" },
		{ "SONG_DANGER", resources::ResourceType::Song, "1800 KB", "-" },
		{ "SONG_SAD", resources::ResourceType::Song, "2100 KB", "-" },
		{ "SONG_VICTORY", resources::ResourceType::Song, "1700 KB", "-" },
		{ "SONG_MENU", resources::ResourceType::Song, "2300 KB", "-" },
		{ "SONG_BONUS", resources::ResourceType::Song, "2500 KB", "-" },
		{ "SONG_CREDITS", resources::ResourceType::Song, "5100 KB", "-" },
		{ "GLOBAL_SCRIPT_INTRO", resources::ResourceType::GlobalScript, "8 KB", "-" },
		{ "VERB_SCRIPT_MAIN", resources::ResourceType::VerbScript, "16 KB", "-" },
		{ "LOCAL_SCRIPT_KITCHEN", resources::ResourceType::LocalScript, "15 KB", "-" },
		{ "LOCAL_SCRIPT_BEDROOM", resources::ResourceType::LocalScript, "11 KB", "-" },
		{ "LOCAL_SCRIPT_GARAGE", resources::ResourceType::LocalScript, "13 KB", "-" },
		{ "LOCAL_SCRIPT_BASEMENT", resources::ResourceType::LocalScript, "18 KB", "-" },
	};

	//---------------------------------------------------------------------
	bool RoomContentWindow::MatchesTabFilter(resources::ResourceType a_eType, int a_iTab) const
	{
		for (resources::ResourceType resourceType : s_aTabs[a_iTab].m_aResourceTypes)
		{
			if (resourceType == resources::ResourceType::Unknown || resourceType == a_eType)
			{
				return true;
			}
		}
		return false;
	}

	//---------------------------------------------------------------------
	int RoomContentWindow::CountResourcesForTab(int a_iTab) const
	{
		int count = 0;
		for (const ResourceEntry& entry : s_aResources)
		{
			if (MatchesTabFilter(entry.eType, a_iTab))
			{
				count++;
			}
		}
		return count;
	}

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
		// TODO: Set ": Room_Name".

		if (ImGui::BeginChild(
			FormatId("", CHILD_ID, "CONTENT").c_str(),
			ImVec2(
				ImGui::GetContentRegionAvail().x,
				ImGui::GetContentRegionAvail().y
			),
			ImGuiChildFlags_Borders
		))
		{
			// Tab bar
			if (ImGui::BeginTabBar(FormatId("", TAB_ID, "RoomResources").c_str(), ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_FittingPolicyScroll))
			{
				float tabIconSize = ImGui::GetFontSize();

				for (int i = 0; i < s_iTabCount; i++)
				{
					int count = CountResourcesForTab(i);
					char label[64];
					snprintf(label, sizeof(label), "  %s (%d)", s_aTabs[i].sLabel, count);

					ImGui::PushStyleColor(ImGuiCol_Tab, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_TabInactive]));
					ImGui::PushStyleColor(ImGuiCol_TabSelected, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_Accent]));
					ImGui::PushStyleColor(ImGuiCol_TabHovered, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_AccentHovered]));
					if (ImGui::BeginTabItem(label))
					{
						m_iSelectedTab = i;
						ImGui::EndTabItem();
					}
					ImGui::PopStyleColor(3);
				}

				// Draw icons on all tabs using internal tab bar state
				ImGuiTabBar* tab_bar = ImGui::GetCurrentTabBar();
				if (tab_bar)
				{
					ImDrawList* drawList = ImGui::GetWindowDrawList();
					for (int i = 0; i < tab_bar->Tabs.Size; i++)
					{
						ImGuiTabItem& tab = tab_bar->Tabs[i];
						if (tab.LastFrameVisible < ImGui::GetFrameCount())
							continue;

						ID3D11ShaderResourceView* pTex = dx11::SVGTextureCache::Get(s_aTabs[i].sIconPath);
						if (!pTex)
							continue;

						float tab_x = tab_bar->BarRect.Min.x + tab.Offset - tab_bar->ScrollingAnim;
						float tab_center_y = tab_bar->BarRect.Min.y + (tab_bar->BarRect.GetHeight() - tabIconSize) * 0.5f;

						drawList->AddImage(
							(ImTextureID)pTex,
							ImVec2(tab_x + 6.0f, tab_center_y),
							ImVec2(tab_x + 6.0f + tabIconSize, tab_center_y + tabIconSize)
						);
					}
				}

				ImGui::EndTabBar();
			}

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

				float iconSize = ImGui::GetFontSize();
				float rowHeight = ImGui::GetFrameHeight();

				for (size_t i = 0; i < s_aResources.size(); i++)
				{
					const ResourceEntry& entry = s_aResources[i];

					if (!MatchesTabFilter(entry.eType, m_iSelectedTab))
					{
						continue;
					}

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

					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));

					bool isSelected = (m_iSelectedRow == static_cast<int>(i));

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
		ImGui::EndChild();
	}
}