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
	class InfoPanelWindow : public LoggerDependentWindow
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
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;
	};
}