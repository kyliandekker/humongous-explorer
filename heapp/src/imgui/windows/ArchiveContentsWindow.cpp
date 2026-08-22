#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "dx11/SVGTextureCache.h"
#include "win32/winfile.h"
#include "file/file.h"
#include "resources/ResourceType.h"
#include "resources/ResourceFactory.h"
#include "resources/Resource.h"
#include "resources/UIHelpers.h"
#include "editor/Workspace.h"
#include "parsing/ChunkParser.h"
#include "parsing/ChunkIDs.h"
#include "core/Memory.h"

#include "core/DataStream.h"
#include "imgui/ImGuiSystem.h"
#include "imgui/views/FileEntryView.h"
#include "imgui/views/ResourceFileEntryView.h"
#include "imgui/views/SearchBar.h"
#include "imgui/Helpers.h"

#include "utils/string_extensions.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	struct DisplayableChunkNode
	{
		parsing::Chunk* m_pChunk = nullptr;
		resources::ResourceType m_eResourceType = resources::ResourceType::Unknown;
		bool m_bVisible = true;
		std::vector<std::unique_ptr<DisplayableChunkNode>> m_aChildren;
	};

	//---------------------------------------------------------------------
	static void CollectDisplayableChunks(
		archive::ArchiveType a_ArchiveType,
		parsing::Chunk& a_Parent,
		std::vector<std::unique_ptr<DisplayableChunkNode>>& a_Out
	)
	{
		const auto& displayable = resources::GetDisplayableChunks(a_ArchiveType);
		for (auto& child : a_Parent.GetChildren())
		{
			std::string tag = child->GetTag();
			if (displayable.count(tag))
			{
				auto node = std::make_unique<DisplayableChunkNode>();
				node->m_pChunk = child.get();
				node->m_eResourceType = displayable.at(tag).m_eResourceType;
				node->m_bVisible = displayable.at(tag).m_bVisible;
				CollectDisplayableChunks(a_ArchiveType, *child, node->m_aChildren);
				a_Out.push_back(std::move(node));
			}
			else
			{
				CollectDisplayableChunks(a_ArchiveType, *child, a_Out);
			}
		}
	}

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	ArchiveContentsWindow::ArchiveContentsWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "ARCHIVE CONTENTS", "ArchiveContentsWindow"),
		m_SearchBar("ArchiveSearchbar", "Search archives...")
	{}

	//---------------------------------------------------------------------
	bool imgui::ArchiveContentsWindow::OnInitialized()
	{
		GetWorkspace().GetArchivesChanged() += std::bind(&ArchiveContentsWindow::OnArchivesChanged, this);

		return true;
	}

	//---------------------------------------------------------------------
	std::vector<std::string> ParseRNAM(parsing::Chunk* a_pChunk)
	{
		std::vector<std::string> roomNames;

		const size_t rnamEnd = a_pChunk->ChunkSize();
		size_t pos = 0;

		while (pos < rnamEnd)
		{
			uint16_t roomNumber;
			memcpy(&roomNumber, a_pChunk->GetData().dataAs<unsigned char>() + pos, sizeof(roomNumber));
			pos += sizeof(roomNumber);

			std::string roomName;

			while (pos < rnamEnd && a_pChunk->GetData()[pos] != '\0')
			{
				roomName += a_pChunk->GetData()[pos];
				++pos;
			}

			if (pos < rnamEnd)
				++pos; // skip '\0'

			roomNames.push_back(roomName);
		}

		return roomNames;
	}

	//---------------------------------------------------------------------
	static std::vector<std::unique_ptr<FileEntryView>> BuildFileEntryViews(
		std::vector<std::unique_ptr<DisplayableChunkNode>>& a_Nodes,
		std::vector<std::string>& a_RoomNames,
		size_t& a_RoomIndex,
		std::unordered_map<std::string, size_t> a_mEntryCountMap
	)
	{
		std::vector<std::unique_ptr<FileEntryView>> views;
		for (auto& node : a_Nodes)
		{
			std::string tag = node->m_pChunk->GetTag();

			a_mEntryCountMap[tag]++;

			std::string resName = resources::GetNameFromResourceType(node->m_eResourceType) + " " + std::to_string(a_mEntryCountMap[tag]);
			if (tag == parsing::LFLF_CHUNK_ID)
			{
				resName = std::to_string(a_RoomIndex + 1) + ". " + (a_RoomNames.size() > a_RoomIndex ? a_RoomNames[a_RoomIndex] : "");
				a_RoomIndex++;
			}

			std::string resIcon = resources::GetIconFromResourceType(node->m_eResourceType);

			std::vector<std::unique_ptr<FileEntryView>> children;
			if (!node->m_aChildren.empty())
			{
				children = BuildFileEntryViews(node->m_aChildren, a_RoomNames, a_RoomIndex, a_mEntryCountMap);
			}

			auto view = std::make_unique<TreeFileEntryView>(
				MakeRows(
					MakeIconRow(resIcon),
					MakeNameRow(resName)
				),
				std::move(children)
			);
			view->m_sName = resName;
			view->m_pChunk = node->m_pChunk;
			view->m_bVisible = node->m_bVisible;
			views.push_back(std::move(view));
		}
		return views;
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::RebuildArchiveViews()
	{
		m_aArchiveViews.clear();

		const auto& archives = GetWorkspace().GetArchiveSet().GetArchives();

		archive::Archive* he0 = nullptr;
		archive::Archive* a = nullptr;
		for (const std::unique_ptr<archive::Archive>& archiveEntry : archives)
		{
			if (archiveEntry->GetType() == archive::ArchiveType::A)
			{
				a = archiveEntry.get();
			}
			else if (archiveEntry->GetType() == archive::ArchiveType::HE0)
			{
				he0 = archiveEntry.get();
			}
		}

		std::vector<std::string> roomNames;
		size_t roomIndex = 0;

		if (he0 && a)
		{
			parsing::Chunk* rnam = he0->GetRoot().TryFindChild(parsing::RNAM_CHUNK_ID);
			if (rnam)
			{
				roomNames = ParseRNAM(rnam);
			}
		}

		for (size_t i = 0; i < archives.size(); ++i)
		{
			std::unordered_map<std::string, size_t> entryCountMap;

			const archive::Archive& archive = *archives[i];
			std::string name = archive.GetName();

			std::vector<std::unique_ptr<DisplayableChunkNode>> displayableNodes;
			CollectDisplayableChunks(archive.GetType(), const_cast<parsing::Chunk&>(archive.GetRoot()), displayableNodes);

			std::vector<std::unique_ptr<FileEntryView>> children;
			if (!displayableNodes.empty())
			{
				children = BuildFileEntryViews(displayableNodes, roomNames, roomIndex, entryCountMap);
			}

			auto archiveView = std::make_unique<TreeFileEntryView>(
				MakeRows(
					MakeIconRow(resources::GetIconFromArchiveType(archive.GetType())),
					MakeNameRow(name),
					MakeCountRow(std::to_string(displayableNodes.size()) + " entries")
				),
				std::move(children)
			);
			archiveView->m_sName = name;
			archiveView->m_pChunk = const_cast<parsing::Chunk*>(&archive.GetRoot());
			archiveView->m_bExpanded = true;

			m_aArchiveViews.push_back(std::move(archiveView));
		}
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::OnArchivesChanged()
	{
		RebuildArchiveViews();
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::Update()
	{
		RenderDropZone();
		ImGui::Spacing();

		if (ImGui::BeginChild(
			FormatId("", CHILD_ID, "ARCHIVE_LIST").c_str(),
			ImVec2(
				ImGui::GetContentRegionAvail().x,
				ImGui::GetContentRegionAvail().y
			),
			ImGuiChildFlags_Borders
		))
		{
			if (m_SearchBar.Render())
			{
				std::string objective = string_extensions::StringToLower(m_SearchBar.GetText());
				for (size_t i = 0; i < m_aArchiveViews.size(); i++)
				{
					m_aArchiveViews[i]->Filter(objective);
				}
			}

			for (const std::unique_ptr<TreeFileEntryView>& view : m_aArchiveViews)
			{
				if (!view->m_bVisible)
				{
					continue;
				}

				view->Render(
					[this, &view](FileEntryView* fileEntry)
					{
						return GetWorkspace().GetSelectedView() == fileEntry;
					},
					[this](FileEntryInteractionType interaction, FileEntryView* fileEntry)
					{
						switch (interaction)
						{
							case FileEntryInteractionType::None:
							{
								break;
							}
							case FileEntryInteractionType::LeftClicked:
							{
								if (TreeFileEntryView* treeView = dynamic_cast<TreeFileEntryView*>(fileEntry))
								{
									GetWorkspace().SetSelectedFileEntryView(treeView);
								}
								break;
							}
							case FileEntryInteractionType::RightClicked:
							{
								break;
							}
							case FileEntryInteractionType::DoubleClicked:
							{
								if (TreeFileEntryView* treeView = dynamic_cast<TreeFileEntryView*>(fileEntry))
								{
									treeView->m_bExpanded = !treeView->m_bExpanded;
								}
								break;
							}
						}
					}
				);
			}
		}
		ImGui::EndChild();
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::RenderDropZone()
	{
		const char* sIconPath = "../icons/icon_drop_file.svg";
		ID3D11ShaderResourceView* pTex = dx11::SVGTextureCache::Get(sIconPath);

		float zoneW = ImGui::GetContentRegionAvail().x;
		float zoneH = 220.0f;

		ImVec2 cursor = ImGui::GetCursorScreenPos();
		ImVec2 zoneMin(cursor.x, cursor.y);
		ImVec2 zoneMax(cursor.x + zoneW, cursor.y + zoneH);

		ImVec2 dropPos = GetImGuiSystem().GetDroppedFilePosition();
		std::string dropped = GetImGuiSystem().ConsumeDroppedFile();
		if (!dropped.empty() &&
			dropPos.x >= zoneMin.x && dropPos.x <= zoneMax.x &&
			dropPos.y >= zoneMin.y && dropPos.y <= zoneMax.y)
		{
			if (GetWorkspace().GetArchiveSet().LoadArchives(dropped))
			{
				GetWorkspace().GetArchivesChanged().invoke();
			}
		}

		bool hovered = false;
		if (ImGui::InvisibleButton(FormatId("", BUTTON_ID, "DROP_ZONE_ARCHIVES").c_str(), ImVec2(zoneW, zoneH)))
		{
			fs::path selected;
			std::vector<COMDLG_FILTERSPEC> filters = {
				{ L"HE Archive", L"*.(A);*.HE0;*.HE1;*.HE2;*.HE3;*.HE4;*.HE7;*.HE8" },
				{ L"All Files", L"*.*" }
			};
			if (file::PickFile(selected, filters))
			{
				if (GetWorkspace().GetArchiveSet().LoadArchives(selected.string()))
				{
					GetWorkspace().GetArchivesChanged().invoke();
				}
			}
		}
		hovered = ImGui::IsItemHovered();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->PushClipRect(zoneMin, zoneMax, true);

		ImU32 bgColor = hovered ? IM_COL32(70, 70, 90, 180) : IM_COL32(45, 45, 55, 100);
		ImU32 borderColor = hovered ? IM_COL32(150, 150, 200, 255) : IM_COL32(100, 100, 120, 200);

		drawList->AddRectFilled(zoneMin, zoneMax, bgColor, 8.0f);
		drawList->AddRect(zoneMin, zoneMax, borderColor, 8.0f, ImDrawFlags_RoundCornersAll, hovered ? 2.0f : 1.0f);

		float centerX = cursor.x + zoneW * 0.5f;
		float curY = cursor.y + 14.0f;

		if (pTex)
		{
			float texW = static_cast<float>(dx11::SVGTextureCache::GetWidth(sIconPath));
			float texH = static_cast<float>(dx11::SVGTextureCache::GetHeight(sIconPath));
			float iconSize = 64.0f;
			float iconW = iconSize;
			float iconH = iconSize * (texH / texW);
			float iconX = centerX - iconW * 0.5f;
			drawList->AddImage((ImTextureID)pTex, ImVec2(iconX, curY), ImVec2(iconX + iconW, curY + iconH));
			curY += iconH + 8.0f;
		}

		const char* subtitle = "(.A, .HE0, .HE2, .HE3, .HE4)";
		ImVec2 subSize = ImGui::CalcTextSize(subtitle);
		drawList->AddText(ImVec2(centerX - subSize.x * 0.5f, curY), IM_COL32(140, 140, 160, 255), subtitle);
		curY += subSize.y + 4.0f;

		const char* hint = "You can also drop a file from Windows Explorer";
		ImVec2 hintSize = ImGui::CalcTextSize(hint);
		drawList->AddText(ImVec2(centerX - hintSize.x * 0.5f, curY), IM_COL32(110, 110, 130, 255), hint);

		drawList->PopClipRect();
	}
}
