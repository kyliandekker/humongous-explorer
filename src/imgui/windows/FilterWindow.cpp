#include "./FilterWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "imgui/ImGuiSetup.h"
#include "imgui/views/ResourceFileEntryView.h"

#include "editor/Workspace.h"

#include "resources/ResourceType.h"

namespace humongousexplorer::imgui
{
	static std::vector<std::unique_ptr<ResourceFileEntryView>> s_aFilters;

	//---------------------------------------------------------------------
	// FilterWindow
	//---------------------------------------------------------------------
	FilterWindow::FilterWindow()
		: BaseWindow(ImGuiWindowFlags_NoCollapse, "FILTER BY TYPE", "FilterWindow")
	{}

	//---------------------------------------------------------------------
	bool FilterWindow::Initialize()
	{
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::Unknown));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::RoomBackground));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::RoomImage));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::Talkie));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::Song));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::SFX));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::LocalScript));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::GlobalScript));
		s_aFilters.push_back(MakeResourceEntryView(resources::ResourceType::VerbScript));

		UpdateResourceCount();

		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void FilterWindow::Update()
	{
		for (auto& view : s_aFilters)
		{
			FileEntryInteractionType interaction = view->Render([&view]()
			{
				return GetWorkspace().GetResourceTypeFilter() == view->m_ResourceType;
			});

			if (interaction == FileEntryInteractionType::LeftClicked)
			{
				GetWorkspace().SetResourceTypeFilter(view->m_ResourceType);
			}
		}
	}

	//---------------------------------------------------------------------
	void imgui::FilterWindow::UpdateResourceCount()
	{
		for (auto& view : s_aFilters)
		{
			if (view->m_aRows.size() < 2)
			{
				continue;
			}

			if (auto* textEntry = dynamic_cast<TextRowEntry*>(view->m_aRows[2].get()))
			{
				textEntry->m_sText = "1";
			}
		}
	}
}
