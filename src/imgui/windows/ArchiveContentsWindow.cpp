#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "dx11/SVGTextureCache.h"
#include "file/file_abstractions.h"
#include "resources/ResourceType.h"
#include "resources/ArchiveType.h"
#include "resources/ResourceFactory.h"
#include "resources/Resource.h"
#include "editor/Workspace.h"
#include "parsing/ChunkParser.h"
#include "resources/ArchiveEntry.h"

#include "core/DataStream.h"
#include "imgui/ImGuiSetup.h"
#include "imgui/views/FileEntryView.h"
#include "imgui/views/ResourceFileEntryView.h"
#include "imgui/views/SearchBar.h"
#include "imgui/Helpers.h"

#include "utils/string_extensions.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	using ChunkPair = std::pair<parsing::Chunk*, resources::DisplayableChunk>;
	//---------------------------------------------------------------------
	static void CollectDisplayableChunks(
		resources::ArchiveType a_ArchiveType,
		parsing::Chunk& a_Parent,
		std::vector<ChunkPair>& a_Out
	)
	{
		const auto& displayable = resources::GetDisplayableChunks(a_ArchiveType);
		for (auto& child : a_Parent.GetChildren())
		{
			std::string tag(child->GetTag(), 4);
			if (displayable.count(tag))
			{
				a_Out.push_back({ child.get(), displayable.at(tag)});
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
	void ArchiveContentsWindow::RebuildArchiveViews()
	{
		m_aArchiveViews.clear();

		const auto& archives = GetWorkspace().GetArchives();

		for (size_t a = 0; a < archives.size(); ++a)
		{
			const resources::ArchiveEntry& archiveEntry = *archives[a];
			std::string name = archiveEntry.GetPath().filename().string();

			std::vector<ChunkPair> displayableChunks;
			CollectDisplayableChunks(archiveEntry.GetType(), const_cast<parsing::Chunk&>(archiveEntry.GetRoot()), displayableChunks);

			std::vector<std::unique_ptr<FileEntryView>> children;
			for (size_t i = 0; i < displayableChunks.size(); ++i)
			{
				const ChunkPair& displayableChunk = displayableChunks[i];

				std::string resName = resources::GetNameFromResourceType(displayableChunk.second.m_eResourceType);
				std::string resIcon = resources::GetIconFromResourceType(displayableChunk.second.m_eResourceType);
				size_t size = displayableChunk.first->ChunkSize();

				auto child = std::make_unique<TreeFileEntryView>(
					MakeRows(
						MakeIconRow(resIcon),
						MakeNameRow(resName)
					)
				);
				child->m_pChunk = displayableChunk.first;
				child->m_bVisible = displayableChunk.second.m_bVisible;
				children.push_back(std::move(child));
			}

			auto archiveView = std::make_unique<TreeFileEntryView>(
				MakeRows(
					MakeIconRow(resources::GetIconFromArchiveType(archiveEntry.GetType())),
					MakeNameRow(name),
					MakeCountRow(std::to_string(displayableChunks.size()) + " entries")
				),
				std::move(children)
			);
			archiveView->m_pChunk = const_cast<parsing::Chunk*>(&archiveEntry.GetRoot());
			archiveView->m_bExpanded = true;

			m_aArchiveViews.push_back(std::move(archiveView));
		}

		resources::ArchiveEntry* he0;
		resources::ArchiveEntry* a;
		for (const std::unique_ptr<resources::ArchiveEntry>& archiveEntry : archives)
		{
			if (archiveEntry->GetType() == resources::ArchiveType::A)
			{
				a = archiveEntry.get();
			}
			else if (archiveEntry->GetType() == resources::ArchiveType::HE0)
			{
				he0 = archiveEntry.get();
			}
		}

		if (!he0 || !a)
		{
			return;
		}

		parsing::Chunk* rnam = he0->GetRoot().TryFindChild(parsing::RNAM_CHUNK_ID);
		printf("Test");
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
								core::DataStream data;
								fileEntry->m_pChunk->GetRoot()->Build(data);
								file::SaveFile("C:/ekkes/aah.txt", data);
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

		ImVec2 dropPos = imgui::GetDroppedFilePosition();
		std::string dropped = imgui::ConsumeDroppedFile();
		if (!dropped.empty() &&
			dropPos.x >= zoneMin.x && dropPos.x <= zoneMax.x &&
			dropPos.y >= zoneMin.y && dropPos.y <= zoneMax.y)
		{
			GetWorkspace().LoadArchives(dropped);
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
				GetWorkspace().LoadArchives(selected.string());
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
