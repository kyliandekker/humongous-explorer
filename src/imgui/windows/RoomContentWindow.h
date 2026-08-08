#pragma once

#include "imgui/windows/BaseWindow.h"
#include "imgui/views/SearchBar.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// RoomContentWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting resources in a room.
	/// </summary>
	class RoomContentWindow : public BaseWindow
	{
	public:
		/// <summary>
		/// Constructs a room content window.
		/// </summary>
		RoomContentWindow();

		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool Initialize() override;

		/// <summary>
		/// Renders the room content window.
		/// </summary>
		void Update() override;
	private:
		SearchBar m_SearchBar;
		int m_iSortColumn = -1;
		bool m_bSortAscending = true;
		int m_iSelectedRow = -1;
	};
}