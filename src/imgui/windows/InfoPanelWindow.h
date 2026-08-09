#pragma once

#include "imgui/windows/HEBaseWindow.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// InfoPanelWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting specific archives.
	/// </summary>
	class InfoPanelWindow : public HEBaseWindow
	{
	public:
		/// <summary>
		/// Constructs an explorer window.
		/// </summary>
		InfoPanelWindow();

		/// <summary>
		/// Renders the explorer window.
		/// </summary>
		void Update() override;

		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool Initialize() override;
	};
}