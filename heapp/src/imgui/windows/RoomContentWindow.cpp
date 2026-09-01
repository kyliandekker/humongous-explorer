#include "RoomContentWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>
#include <vector>
#include <algorithm>

#include "helib/core/Memory.h"
#include "imgui/ImGuiSystem.h"
#include "imgui/views/SearchBar.h"
#include "resources/ResourceType.h"
#include "dx11/SVGTextureCache.h"
#include "utils/string_extensions.h"
#include "imgui/Helpers.h"
#include "editor/Workspace.h"
#include "imgui/views/FileEntryView.h"

#include "resources/Resource.h"
#include "resources/ResourceFactory.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	struct TabInfo
	{
		const char* sLabel;
		std::string sIconPath;
		std::vector<resources::ResourceType> m_aResourceTypes;
	};

	//---------------------------------------------------------------------
	static const TabInfo s_aTabs[] =
	{
		{ "All Files",  resources::GetIconFromResourceType(resources::ResourceType::Unknown), { resources::ResourceType::Unknown } },
		{ "Room Backgrounds",  resources::GetIconFromResourceType(resources::ResourceType::RoomBackground), { resources::ResourceType::RoomBackground } },
		{ "Room Images",  resources::GetIconFromResourceType(resources::ResourceType::RoomImage), { resources::ResourceType::RoomImage, resources::ResourceType::RoomImageLayer } },
		{ "SFX",	 resources::GetIconFromResourceType(resources::ResourceType::SFX), {resources::ResourceType::SFX}},
		{ "Talkies",	 resources::GetIconFromResourceType(resources::ResourceType::Talkie), { resources::ResourceType::Talkie } },
		{ "Songs",	 resources::GetIconFromResourceType(resources::ResourceType::Song), { resources::ResourceType::Song } },
		{ "Scripts", resources::GetIconFromResourceType(resources::ResourceType::GlobalScript), { resources::ResourceType::GlobalScript, resources::ResourceType::LocalScript, resources::ResourceType::VerbScript } },
	};
	static const int s_iTabCount = sizeof(s_aTabs) / sizeof(s_aTabs[0]);

	//---------------------------------------------------------------------
	struct ResourceEntry
	{
		size_t m_iOrder;
		resources::Resource* m_pResource = nullptr;
	};

	//---------------------------------------------------------------------
	static std::vector<std::unique_ptr<ResourceEntry>> s_aResourceEntries =
	{};

	static std::vector<std::unique_ptr<resources::Resource>> s_aResources =
	{};

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
		for (const std::unique_ptr<ResourceEntry>& entry : s_aResourceEntries)
		{
			if (MatchesTabFilter(entry->m_pResource->GetResourceType(), a_iTab))
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
		: LoggerDependentWindow(ImGuiWindowFlags_NoCollapse, "CONTENT EXPLORER", "ContentWindow"),
		m_SearchBar("RoomSearchbar", "Search resources...")
	{
	}

	//---------------------------------------------------------------------
	bool RoomContentWindow::OnInitialized()
	{
		GetWorkspace().GetSelectedViewObs().OnChanged() += std::bind(&RoomContentWindow::OnSelectedViewChanged, this, std::placeholders::_1, std::placeholders::_2);
		return true;
	}

	//---------------------------------------------------------------------
	void RoomContentWindow::OnSelectedViewChanged(const imgui::TreeFileEntryView* oldView, const imgui::TreeFileEntryView* newView)
	{
		m_sRoomName = "";
		GetWorkspace().SetSelectedResource(nullptr);

		s_aResourceEntries.clear();
		s_aResources.clear();

		if (!newView)
		{
			return;
		}

		for (size_t i = 0; i < newView->m_aChildren.size(); i++)
		{
			auto& view = newView->m_aChildren[i];
			if (!view)
			{
				continue;
			}

			s_aResources.push_back(std::move(resources::ResourceFactory::GetResource(view->m_pChunk, view->m_sName)));
		}

		m_sRoomName = newView->m_sName;

		for (size_t i = 0; i < s_aResources.size(); i++)
		{
			auto& resource = s_aResources[i];

			std::unique_ptr<ResourceEntry> resourceEntry = std::make_unique<ResourceEntry>();
			resourceEntry->m_pResource = resource.get();
			resourceEntry->m_iOrder = i;
			s_aResourceEntries.push_back(std::move(resourceEntry));
		}
	}

	//---------------------------------------------------------------------
	int CompareSize(size_t a, size_t b)
	{
		if (a < b)
		{
			return -1;
		}
		if (a > b)
		{
			return 1;
		}
		return 0;
	}

	//---------------------------------------------------------------------
	void RoomContentWindow::Update()
	{
		if (!m_sRoomName.empty())
		{
			m_sName = "CONTENT EXPLORER (" + m_sRoomName + ")";
		}
		else
		{
			m_sName = "CONTENT EXPLORER";
		}

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
					for (int i = 0; i < s_iTabCount; i++)
					{
						ImGuiTabItem& tab = tab_bar->Tabs[i];
						if (tab.LastFrameVisible < ImGui::GetFrameCount())
						{
							continue;
						}

						ID3D11ShaderResourceView* pTex = dx11::SVGTextureCache::Get(s_aTabs[i].sIconPath);
						if (!pTex)
						{
							continue;
						}

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

			if (ImGui::BeginTable("ResourceTable", 8,
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
				ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthStretch, 1.0f);
				ImGui::TableHeadersRow();

				float iconSize = ImGui::GetFontSize();
				float rowHeight = ImGui::GetFrameHeight();

				for (size_t i = 0; i < s_aResourceEntries.size(); i++)
				{
					const std::unique_ptr<ResourceEntry>& entry = s_aResourceEntries[i];

					if (!MatchesTabFilter(entry->m_pResource->GetResourceType(), m_iSelectedTab))
					{
						continue;
					}

					if (!filter.empty())
					{
						std::string lowerName = string_extensions::StringToLower(entry->m_pResource->GetName());
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

					auto selectedRes = GetWorkspace().GetSelectedResource();
					auto currentRes = s_aResources[i].get();
					bool isSelected = (selectedRes != nullptr && currentRes == selectedRes);

					if (ImGui::Selectable(
						FormatId("", SELECTABLE_ID, m_sName, "ROW", std::to_string(i)).c_str(),
						isSelected,
						ImGuiSelectableFlags_SpanAllColumns,
						ImVec2(0.0f, 0.0f)
					))
					{
						GetWorkspace().SetSelectedResource(currentRes);
						currentRes->Open();
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
						ID3D11ShaderResourceView* pTex = dx11::SVGTextureCache::Get(resources::GetIconFromResourceType(entry->m_pResource->GetResourceType()).c_str());
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
					ImGui::TextUnformatted(entry->m_pResource->GetName().c_str());

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(resources::GetNameFromResourceType(entry->m_pResource->GetResourceType()).c_str());

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(entry->m_pResource->GetSize().c_str());

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(entry->m_pResource->GetDimensions().c_str());

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(entry->m_pResource->GetDurationStr().c_str());
				}

				if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs())
				{
					if (sortSpecs->SpecsDirty && sortSpecs->SpecsCount > 0)
					{
						const ImGuiTableColumnSortSpecs& spec = sortSpecs->Specs[0];
						m_iSortColumn = spec.ColumnIndex;
						m_bSortAscending = spec.SortDirection == ImGuiSortDirection_Ascending;
						sortSpecs->SpecsDirty = false;

						std::sort(s_aResourceEntries.begin(), s_aResourceEntries.end(), [this](const std::unique_ptr<ResourceEntry>& a, const std::unique_ptr<ResourceEntry>& b)
							{
								int cmp = 0;
								switch (m_iSortColumn)
								{
									case 0:
									case 1:
									{
										cmp = CompareSize(a->m_iOrder, b->m_iOrder);
										break;
									}
									case 2:
									{
										cmp = a->m_pResource->GetName().compare(b->m_pResource->GetName());
										break;
									}
									case 3:
									{
										cmp = resources::GetNameFromResourceType(a->m_pResource->GetResourceType()).compare(resources::GetNameFromResourceType(b->m_pResource->GetResourceType()));
										break;
									}
									case 4:
									{
										cmp = a->m_pResource->GetSize().compare(b->m_pResource->GetSize());
										break;
									}
									case 5:
									{
										cmp = a->m_pResource->GetDimensions().compare(b->m_pResource->GetDimensions());
										break;
									}
									case 6:
									{
										cmp = a->m_pResource->GetDurationStr().compare(b->m_pResource->GetDurationStr());
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