#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "imgui/ImGuiSetup.h"
#include "resources/ResourceType.h"
#include "resources/ArchiveType.h"
#include "imgui/views/ResourceFileEntryView.h"
#include "imgui/views/SearchBar.h"
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
	std::unique_ptr<TreeFileEntryView> MakeRoomEntryView(
		const std::string& a_sName,
		const std::string& a_sCount = "UNINITIALIZED"
	)
	{
		return std::make_unique<TreeFileEntryView>(
			MakeRows(
				MakeIconRow(resources::GetIconFromResourceType(resources::ResourceType::Room)),
				MakeNameRow(a_sName),
				MakeCountRow(a_sCount)
			),
			[]()
			{
				std::vector<std::unique_ptr<FileEntryView>> children;
				children.push_back(MakeArchiveResourceEntryView("Test.png", resources::ResourceType::RoomImage));
				children.push_back(MakeArchiveResourceEntryView("Aaah.png", resources::ResourceType::RoomImage));
				return children;
			}()
		);
	}

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	ArchiveContentsWindow::ArchiveContentsWindow() : BaseWindow(ImGuiWindowFlags_NoCollapse, "ARCHIVE CONTENTS", "ArchiveContentsWindow"),
		m_SearchBar("ArchiveSearchbar", "Search archives...")
	{
	}

	//---------------------------------------------------------------------
	bool imgui::ArchiveContentsWindow::Initialize()
	{
		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.(a)", resources::ArchiveType::A));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE0", resources::ArchiveType::HE0,
			[]()
			{
				std::vector<std::unique_ptr<FileEntryView>> children;
				children.push_back(MakeRoomEntryView("Helogo"));
				children.push_back(MakeRoomEntryView("Interface"));
				children.push_back(MakeRoomEntryView("Saveload"));
				children.push_back(MakeRoomEntryView("Spywatch"));
				children.push_back(MakeRoomEntryView("Mobcom"));
				return children;
			}()
		));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE1", resources::ArchiveType::HE1));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE2", resources::ArchiveType::HE2));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE3", resources::ArchiveType::HE3));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE4", resources::ArchiveType::HE4));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE7", resources::ArchiveType::HE7));

		s_aArchives.push_back(MakeArchiveEntryView("SPYOZON.HE8", resources::ArchiveType::HE8));

		for (size_t i = 0; i < s_aArchives.size(); i++)
		{
			s_aArchives[i]->Filter("");
		}

		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::Update()
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

			FileEntryInteractionType interaction = view->Render([this, &view]()
				{
					return m_pFilterFileEntryView == view.get();
				});

			if (interaction == FileEntryInteractionType::LeftClicked)
			{
				m_pFilterFileEntryView = view.get();
			}
		}
	}
}
