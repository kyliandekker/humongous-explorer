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
	std::unique_ptr<ResourceFileEntryView> MakeFilterEntryView(
		resources::ResourceType a_eType,
		const std::string& a_sCount = "UNINITIALIZED"
	)
	{
		return std::make_unique<ResourceFileEntryView>(
			a_eType,
			MakeRows(
				MakeIconRow(resources::GetIconFromResourceType(a_eType)),
				MakeNameRow(resources::GetNameFromResourceType(a_eType)),
				MakeCountRow(a_sCount)
			)
		);
	}

	//---------------------------------------------------------------------
	// FilterWindow
	//---------------------------------------------------------------------
	FilterWindow::FilterWindow()
		: BaseWindow(ImGuiWindowFlags_NoCollapse, "FILTER BY TYPE", "FilterWindow")
	{}

	//---------------------------------------------------------------------
	bool FilterWindow::Initialize()
	{
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::Unknown));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::RoomBackground));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::RoomImage));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::Talkie));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::Song));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::SFX));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::LocalScript));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::GlobalScript));
		s_aFilters.push_back(MakeFilterEntryView(resources::ResourceType::VerbScript));

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
