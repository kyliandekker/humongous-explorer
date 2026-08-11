#pragma once

#include "imgui/windows/HEBaseWindow.h"
#include "imgui/views/SearchBar.h"
#include "resources/ResourceType.h"

namespace humongousexplorer::imgui
{
	class TreeFileEntryView;

	//---------------------------------------------------------------------
	// RoomContentWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting resources in a room.
	/// </summary>
	class RoomContentWindow : public HEBaseWindow
	{
	public:
		/// <summary>
		/// Constructs a room content window.
		/// </summary>
		RoomContentWindow();

		/// <summary>
		/// Renders the room content window.
		/// </summary>
		void Update() override;
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		void OnSelectedViewChanged(const imgui::TreeFileEntryView* oldView, const imgui::TreeFileEntryView* newView);

		SearchBar m_SearchBar;
		int m_iSortColumn = -1;
		bool m_bSortAscending = true;
		int m_iSelectedRow = -1;
		int m_iSelectedTab = 0;

		bool MatchesTabFilter(resources::ResourceType a_eType, int a_iTab) const;
		int CountResourcesForTab(int a_iTab) const;
	};
}