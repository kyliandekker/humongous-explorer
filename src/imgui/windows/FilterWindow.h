#pragma once

#include "imgui/windows/HEBaseWindow.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// FilterWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for filtering by type.
	/// </summary>
	class FilterWindow : public HEBaseWindow
	{
	public:
		/// <summary>
		/// Constructs an explorer window.
		/// </summary>
		FilterWindow();

		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool Initialize() override;

		/// <summary>
		/// Renders the explorer window.
		/// </summary>
		void Update() override;
	private:
		void UpdateResourceCount();
	};
}
