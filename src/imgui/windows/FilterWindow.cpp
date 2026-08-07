#include "./FilterWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "dx11/SVGTextureCache.h"

#include "imgui/ImGuiSetup.h"
#include "imgui/FileEntryView.h"

#include "editor/Workspace.h"

#include "resources/ResourceType.h"

namespace humongousexplorer::imgui
{
	struct FilterFileEntryView : public FileEntryView
	{
		FilterFileEntryView(resources::ResourceType a_ResourceType, const std::string& a_sIcon, const std::vector<RowInfo>& a_aRows) : FileEntryView(a_sIcon, a_aRows),
			m_ResourceType(a_ResourceType)
		{}
		resources::ResourceType m_ResourceType;
	};

	static std::vector<FilterFileEntryView> s_aFileEntries;

	//---------------------------------------------------------------------
	// FilterWindow
	//---------------------------------------------------------------------
	FilterWindow::FilterWindow()
		: BaseWindow(ImGuiWindowFlags_NoCollapse, "FILTER BY TYPE", "FilterWindow")
	{}

	//---------------------------------------------------------------------
	bool imgui::FilterWindow::Initialize()
	{
		ImColor typeColor = IM_COL32(236, 239, 244, 255);
		float typeOffset = 50;

		ImColor countColor = IM_COL32(135, 145, 165, 255);
		float countOffset = 8;

		s_aFileEntries.push_back(
			{
				resources::ResourceType::Unknown,
				"../icons/icon_all_files.svg",
				{
					{
						"All Types",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::RoomBackground,
				"../icons/icon_background.svg",
				{
					{
						"Backgrounds",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::RoomImage,
				"../icons/icon_image.svg",
				{
					{
						"Sprites",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::Talkie,
				"../icons/icon_talkie.svg",
				{
					{
						"Talkies",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::Song,
				"../icons/icon_song.svg",
				{
					{
						"Songs",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::SFX,
				"../icons/icon_sfx.svg",
				{
					{
						"Sfx",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::LocalScript,
				"../icons/icon_local_script.svg",
				{
					{
						"Local Scripts",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::GlobalScript,
				"../icons/icon_global_script.svg",
				{
					{
						"Global Scripts",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		s_aFileEntries.push_back(
			{
				resources::ResourceType::VerbScript,
				"../icons/icon_verb_script.svg",
				{
					{
						"Verb Scripts",
						typeColor,
						typeOffset
					},
					{
						"UNINITIALIZED",
						countColor,
						countOffset,
						RowInfoTextAlignment::Right
					},
				}
			}
		);

		UpdateResourceCount();

		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void FilterWindow::Update()
	{
		for (FilterFileEntryView& view : s_aFileEntries)
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
		for (FilterFileEntryView& view : s_aFileEntries)
		{
			if (view.m_aRows.size() < 1)
			{
				continue;
			}
			view.m_aRows[1].m_sName = "1";
		}
	}
}
