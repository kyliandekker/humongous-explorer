#pragma once

#include "BaseWindow.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// MainWindowDock
	//---------------------------------------------------------------------
	/// <summary>
	/// Main window that allows docking for other windows.
	/// </summary>
	class MainWindowDock : public BaseWindow
	{
	public:
		/// <summary>
		/// Constructs the main window.
		/// </summary>
		MainWindowDock();

		/// <summary>
		/// Renders the element.
		/// </summary>
		void Update() override;
	};
}