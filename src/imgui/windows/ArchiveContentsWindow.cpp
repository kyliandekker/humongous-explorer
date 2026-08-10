#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "resources/ResourceType.h"
#include "resources/ArchiveType.h"

#include "imgui/ImGuiSetup.h"
#include "imgui/views/ResourceFileEntryView.h"
#include "imgui/views/SearchBar.h"
#include "imgui/Helpers.h"

#include "utils/string_extensions.h"

namespace humongousexplorer::imgui
{
	static std::vector<std::unique_ptr<TreeFileEntryView>> s_aArchives;

	//---------------------------------------------------------------------
	std::unique_ptr<TreeFileEntryView> MakeArchiveEntryView(
		const std::string& a_sName,
		resources::ArchiveType a_ArchiveType,
		std::vector<std::unique_ptr<FileEntryView>> a_aChildren = {},
		const std::string& a_sCount = "UNINITIALIZED"
	)
	{
		return std::make_unique<TreeFileEntryView>(
			MakeRows(
				MakeIconRow(resources::GetIconFromArchiveType(a_ArchiveType)),
				MakeNameRow(a_sName),
				MakeCountRow(a_sCount)
			),
			std::move(a_aChildren)
		);
	}

	//---------------------------------------------------------------------
	std::unique_ptr<ResourceFileEntryView> MakeArchiveResourceEntryView(
		const std::string& a_sName,
		resources::ResourceType a_eType,
		const std::string& a_sCount = "UNINITIALIZED"
	)
	{
		return std::make_unique<ResourceFileEntryView>(
			a_eType,
			MakeRows(
				MakeIconRow(resources::GetIconFromResourceType(a_eType)),
				MakeNameRow(a_sName),
				MakeCountRow(a_sCount)
			)
		);
	}

	//---------------------------------------------------------------------
	std::unique_ptr<FileEntryView> MakeRoomEntryView(
		const std::string& a_sName,
		const std::string& a_sCount = "UNINITIALIZED"
	)
	{
		return std::make_unique<FileEntryView>(
			MakeRows(
				MakeIconRow(resources::GetIconFromResourceType(resources::ResourceType::Room)),
				MakeNameRow(a_sName),
				MakeCountRow(a_sCount)
			)
		);
	}

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	ArchiveContentsWindow::ArchiveContentsWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "ARCHIVE CONTENTS", "ArchiveContentsWindow"),
		m_SearchBar("ArchiveSearchbar", "Search archives...")
	{
	}

	//---------------------------------------------------------------------
	bool imgui::ArchiveContentsWindow::Initialize()
	{
		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.(a)", resources::ArchiveType::A, {}, ""));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE0", resources::ArchiveType::HE0,
			[]()
			{
				std::vector<std::unique_ptr<FileEntryView>> children;
				children.push_back(MakeRoomEntryView("Room 001 - Helogo"));
				children.push_back(MakeRoomEntryView("Room 002 - Interface"));
				children.push_back(MakeRoomEntryView("Room 003 - Saveload"));
				children.push_back(MakeRoomEntryView("Room 004 - Spywatch"));
				children.push_back(MakeRoomEntryView("Room 005 - Mobcom"));
				return children;
			}(), "5 ROOMS"
		));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE1", resources::ArchiveType::HE1, {}, "253"));
		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE2", resources::ArchiveType::HE2, {}, "32"));
		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE3", resources::ArchiveType::HE3, {}, "12"));
		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE4", resources::ArchiveType::HE4, {}, "54"));
		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE7", resources::ArchiveType::HE7, {}, "68"));
		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE8", resources::ArchiveType::HE8, {}, "43"));

		return HEBaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::Update()
	{
		if (ImGui::BeginChild(
			FormatId("", CHILD_ID, "ARCHIVE_CONTENTS").c_str(),
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
}
