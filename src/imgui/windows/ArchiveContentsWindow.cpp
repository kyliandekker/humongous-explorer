#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "dx11/SVGTextureCache.h"
#include "file/file_abstractions.h"
#include "resources/ResourceType.h"
#include "resources/ArchiveType.h"
#include "editor/Workspace.h"
#include "parsing/HEParser.h"

#include "core/Memory.h"

#include "imgui/ImGuiSetup.h"
#include "imgui/views/ResourceFileEntryView.h"
#include "imgui/views/SearchBar.h"
#include "imgui/Helpers.h"

#include "utils/string_extensions.h"

namespace humongousexplorer::imgui
{
	static std::vector<std::unique_ptr<TreeFileEntryView>> s_aArchives;

	using ChunkPair = std::pair<const parsing::Chunk*, resources::DisplayableChunk>;
	//---------------------------------------------------------------------
	static void CollectDisplayableChunks(
		resources::ArchiveType a_ArchiveType,
		const parsing::Chunk& a_Parent,
		std::vector<ChunkPair>& a_Out
	)
	{
		const auto& displayable = resources::GetDisplayableChunks(a_ArchiveType);
		for (const auto& child : a_Parent.m_aChildren)
		{
			std::string tag(child.m_sTag, 4);
			if (displayable.count(tag))
			{
				a_Out.push_back({ &child, displayable.at(tag)});
			}
			else
			{
				CollectDisplayableChunks(a_ArchiveType, child, a_Out);
			}
		}
	}

	//---------------------------------------------------------------------
	static void LoadArchive(const std::string& a_sPath)
	{
		fs::path filePath = a_sPath;
		std::string ext = filePath.extension().string();
		if (!ext.empty() && ext[0] == '.')
		{
			ext = ext.substr(1);
		}

		editor::ArchiveData archive;
		archive.m_sPath = filePath.string();
		archive.m_eType = resources::GetArchiveTypeFromExtension(ext);

		if (archive.m_eType < resources::ArchiveType::HE0)
		{
			return;
		}

		if (!file::LoadFile(filePath, archive.m_Data))
		{
			return;
		}

		if (archive.m_eType == resources::ArchiveType::HE0)
		{
			unsigned char* data = archive.m_Data.dataAs<unsigned char>();
			core::xorShift(data, archive.m_Data.size(), 0x69);
		}

		if (!archive.m_Data.empty())
		{
			parsing::ParseChunks(
				archive.m_Root,
				static_cast<const uint8_t*>(archive.m_Data.data()),
				0
			);
		}

		std::string name = filePath.filename().string();

		std::vector<ChunkPair> displayableChunks;
		CollectDisplayableChunks(archive.m_eType, archive.m_Root, displayableChunks);

		std::vector<std::unique_ptr<FileEntryView>> children;
		for (const ChunkPair& displayableChunk : displayableChunks)
		{
			std::string tag(displayableChunk.first->m_sTag, 4);
			size_t size = displayableChunk.first->ChunkSize();
			std::unique_ptr<TreeFileEntryView> child = std::make_unique<TreeFileEntryView>(
				MakeRows(
					MakeIconRow(resources::GetIconFromResourceType(displayableChunk.second.m_eResourceType)),
					MakeNameRow(resources::GetNameFromResourceType(displayableChunk.second.m_eResourceType)),
					MakeCountRow(std::to_string(size) + " bytes")
				)
			);
			child->m_pChunk = displayableChunk.first;
			child->m_bVisible = displayableChunk.second.m_bVisible;
			children.push_back(std::move(child));
		}

		auto archiveView = std::make_unique<TreeFileEntryView>(
			MakeRows(
				MakeIconRow(resources::GetIconFromArchiveType(archive.m_eType)),
				MakeNameRow(name),
				MakeCountRow(std::to_string(displayableChunks.size()) + " entries")
			),
			std::move(children)
		);
		archiveView->m_bExpanded = true;

		s_aArchives.push_back(std::move(archiveView));

		GetWorkspace().SetSelectedFileEntryView(s_aArchives[s_aArchives.size() - 1].get());
		GetWorkspace().AddArchive(std::move(archive));
	}

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	ArchiveContentsWindow::ArchiveContentsWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "ARCHIVE CONTENTS", "ArchiveContentsWindow"),
		m_SearchBar("ArchiveSearchbar", "Search archives...")
	{
	}

	//---------------------------------------------------------------------
	bool imgui::ArchiveContentsWindow::OnInitialized()
	{
		return true;
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
				for (size_t i = 0; i < s_aArchives.size(); i++)
				{
					s_aArchives[i]->Filter(objective);
				}
			}

			for (const std::unique_ptr<TreeFileEntryView>& view : s_aArchives)
			{
				if (!view->m_bVisible)
				{
					continue;
				}

				view->Render(
					[this, &view](FileEntryView* fileEntry)
					{
						return m_pFilterFileEntryView == fileEntry;
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
								m_pFilterFileEntryView = fileEntry;
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
								else
								{
									// TODO: Read room.
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
			LoadArchive(dropped);
		}

		bool hovered = false;
		if (ImGui::InvisibleButton("##drop_zone", ImVec2(zoneW, zoneH)))
		{
			fs::path selected;
			std::vector<COMDLG_FILTERSPEC> filters = {
				{ L"HE Archive", L"*.HE0;*.HE1;*.HE2;*.HE3;*.HE4;*.HE7;*.HE8;*.A" },
				{ L"All Files", L"*.*" }
			};
			if (file::PickFile(selected, filters))
			{
				LoadArchive(selected.string());
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
