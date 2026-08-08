#pragma once

#include "imgui/windows/BaseWindow.h"

namespace humongousexplorer::imgui
{
	class TreeFileEntryView;

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting specific archives.
	/// </summary>
	class ArchiveContentsWindow : public BaseWindow
	{
	public:
		/// <summary>
		/// Constructs an explorer window.
		/// </summary>
		ArchiveContentsWindow();

		/// <summary>
		/// Renders the explorer window.
		/// </summary>
		void Update() override;

		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool Initialize() override;
	private:
		TreeFileEntryView* m_pFilterFileEntryView = nullptr; // TODO: move
	};
}