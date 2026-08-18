#pragma once

#include "imgui/windows/HEBaseWindow.h"
#include "imgui/views/SearchBar.h"
#include "imgui/views/FileEntryView.h"
#include <vector>
#include <memory>

namespace humongousexplorer::imgui
{
	class FileEntryView;

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	/// <summary>
	/// A window for inspecting specific archives.
	/// </summary>
	class ArchiveContentsWindow : public HEBaseWindow
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
	private:
		/// <summary>
		/// Initializes all behaviours and values for the window.
		/// </summary>
		/// <returns>True if initialization is successful, otherwise false.</returns>
		bool OnInitialized() override;

		void RenderDropZone();
		void RebuildArchiveViews();

		void OnArchivesChanged();

		std::vector<std::unique_ptr<TreeFileEntryView>> m_aArchiveViews;
		SearchBar m_SearchBar;
	};
}