#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>
#include <future>

#include "dx11/SVGTextureCache.h"
#include "file/file_abstractions.h"
#include "resources/ResourceType.h"
#include "resources/ArchiveType.h"
#include "editor/Workspace.h"
#include "parsing/HEParser.h"
#include "logger/Logger.h"

#include "core/Memory.h"

#include "imgui/ImGuiSetup.h"
#include "imgui/views/ResourceFileEntryView.h"
#include "imgui/views/SearchBar.h"
#include "imgui/Helpers.h"

#include "utils/string_extensions.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	static std::vector<std::unique_ptr<TreeFileEntryView>> s_aArchives;

	using ChunkPair = std::pair<parsing::Chunk*, resources::DisplayableChunk>;
	//---------------------------------------------------------------------
	static void CollectDisplayableChunks(
		resources::ArchiveType a_ArchiveType,
		parsing::Chunk& a_Parent,
		std::vector<ChunkPair>& a_Out
	)
	{
		const auto& displayable = resources::GetDisplayableChunks(a_ArchiveType);
		for (auto& child : a_Parent.m_aChildren)
		{
			std::string tag(child->m_sTag, 4);
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
	static std::optional<editor::ArchiveData> LoadArchive(const fs::path& a_sPath)
	{
		editor::ArchiveData archive;

		archive.m_sPath = a_sPath;
		archive.m_eType =
			resources::GetArchiveTypeFromExtension(
				a_sPath.extension().string().substr(1)
			);

		if (archive.m_eType < resources::ArchiveType::HE0)
		{
			return std::nullopt;
		}

		if (!file::LoadFile(a_sPath, archive.m_Data))
		{
			return std::nullopt;
		}

		if (!archive.m_Data.empty())
		{
			if (!parsing::ParseChunks(archive.m_Root, archive.m_Data, 0))
			{
				core::Data xorredData = archive.m_Data;

				unsigned char* data = xorredData.dataAs<unsigned char>();

				core::xorShift(data, xorredData.size(), 0x69);

				if (!parsing::ParseChunks(archive.m_Root, xorredData, 0))
				{
					return std::nullopt;
				}
			}
		}

		return archive;
	}

	//---------------------------------------------------------------------
	static void LoadArchives(const std::string& a_sPath)
	{
		GetWorkspace().ClearArchives();

		fs::path path = a_sPath;
		fs::path folder = path.parent_path();

		std::vector<fs::path> paths;

		for (const auto& entry : fs::directory_iterator(folder))
		{
			if (!entry.is_regular_file())
			{
				continue;
			}

			const fs::path& filePath = entry.path();

			std::string extension = filePath.extension().string().substr(1);

			if (resources::GetArchiveTypeFromExtension(extension) < resources::ArchiveType::HE0)
			{
				continue;
			}

			if (filePath.stem().generic_string() != path.stem().generic_string())
			{
				continue;
			}

			paths.push_back(filePath);
		}

		std::vector<std::future<std::optional<editor::ArchiveData>>> futures;

		futures.reserve(paths.size());

		for (const fs::path& filePath : paths)
		{
			futures.emplace_back(std::async(
				std::launch::async,
				LoadArchive,
				filePath
			));
		}

		// Back on the calling/main thread.
		for (size_t i = 0; i < futures.size(); ++i)
		{
			auto archive = futures[i].get();

			if (!archive)
			{
				LOGF(LogSeverity::LOGSEVERITY_ERROR, "Could not load archive: \"%s\"", paths[i].filename().generic_string().c_str());
				continue;
			}

			GetWorkspace().AddArchive(std::move(*archive));
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
		GetWorkspace().GetOnArchiveAdded() += std::bind(&ArchiveContentsWindow::GetOnArchiveAdded, this, std::placeholders::_1);
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
								GetWorkspace().SetSelectedFileEntryView(dynamic_cast<TreeFileEntryView*>(fileEntry));
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
			LoadArchives(dropped);
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
				LoadArchives(selected.string());
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

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::GetOnArchiveAdded(std::unique_ptr<editor::ArchiveData>& a_pArchiveData)
	{
		std::string name = a_pArchiveData->m_sPath.filename().string();

		std::vector<ChunkPair> displayableChunks;
		CollectDisplayableChunks(a_pArchiveData->m_eType, a_pArchiveData->m_Root, displayableChunks);

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
				MakeIconRow(resources::GetIconFromArchiveType(a_pArchiveData->m_eType)),
				MakeNameRow(name),
				MakeCountRow(std::to_string(displayableChunks.size()) + " entries")
			),
			std::move(children)
		);
		archiveView->m_bExpanded = true;

		GetWorkspace().GetOnLoadArchiveProgressed().invoke(100);
		GetWorkspace().GetOnLoadArchiveSuccess().invoke(a_pArchiveData->m_sPath.generic_string());

		s_aArchives.push_back(std::move(archiveView));
	}
}
