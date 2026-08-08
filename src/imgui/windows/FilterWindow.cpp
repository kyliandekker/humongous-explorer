#include "./FilterWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "imgui/ImGuiSetup.h"
#include "imgui/FileEntryView.h"

#include "editor/Workspace.h"

#include "resources/ResourceType.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	struct FilterFileEntryView : public FileEntryView
	{
		FilterFileEntryView(resources::ResourceType a_ResourceType, std::vector<std::unique_ptr<RowEntry>> a_aRows) : FileEntryView(std::move(a_aRows)),
			m_ResourceType(a_ResourceType)
		{}
		resources::ResourceType m_ResourceType;
	};

	static std::vector<FilterFileEntryView> s_aFilters;

	//---------------------------------------------------------------------
	// FilterWindow
	//---------------------------------------------------------------------
	FilterWindow::FilterWindow()
		: BaseWindow(ImGuiWindowFlags_NoCollapse, "FILTER BY TYPE", "FilterWindow")
	{}

	//---------------------------------------------------------------------
	bool imgui::FilterWindow::Initialize()
	{
		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::Unknown,
			MakeRows(MakeIconRow("../icons/icon_all_files.svg"), MakeNameRow("All Types"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::RoomBackground,
			MakeRows(MakeIconRow("../icons/icon_background.svg"), MakeNameRow("Room Backgrounds"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::RoomImage,
			MakeRows(MakeIconRow("../icons/icon_image.svg"), MakeNameRow("Sprites"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::Talkie,
			MakeRows(MakeIconRow("../icons/icon_talkie.svg"), MakeNameRow("Talkies"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::Song,
			MakeRows(MakeIconRow("../icons/icon_song.svg"), MakeNameRow("Songs"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::SFX,
			MakeRows(MakeIconRow("../icons/icon_sfx.svg"), MakeNameRow("SFX"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::LocalScript,
			MakeRows(MakeIconRow("../icons/icon_local_script.svg"), MakeNameRow("Local Scripts"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::GlobalScript,
			MakeRows(MakeIconRow("../icons/icon_global_script.svg"), MakeNameRow("Global Scripts"), MakeCountRow("UNINITIALIZED"))
		));

		s_aFilters.push_back(FilterFileEntryView(
			resources::ResourceType::VerbScript,
			MakeRows(MakeIconRow("../icons/icon_verb_script.svg"), MakeNameRow("Verb Scripts"), MakeCountRow("UNINITIALIZED"))
		));

		UpdateResourceCount();

		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void FilterWindow::Update()
	{
		for (FilterFileEntryView& view : s_aFilters)
		{
			FileEntryInteractionType interaction = view.Render([&view]()
			{
				return GetWorkspace().GetResourceTypeFilter() == view.m_ResourceType;
			});

			if (interaction == FileEntryInteractionType::LeftClicked)
			{
				GetWorkspace().SetResourceTypeFilter(view.m_ResourceType);
			}
		}
	}

	//---------------------------------------------------------------------
	void imgui::FilterWindow::UpdateResourceCount()
	{
		for (FilterFileEntryView& view : s_aFilters)
		{
			if (view.m_aRows.size() < 2)
			{
				continue;
			}

			if (auto* textEntry = dynamic_cast<TextRowEntry*>(view.m_aRows[2].get()))
			{
				textEntry->m_sText = "1";
			}
		}
	}
}
