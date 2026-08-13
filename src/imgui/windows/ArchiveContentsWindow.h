#pragma once

#include <memory>

#include "imgui/windows/HEBaseWindow.h"
#include "imgui/views/SearchBar.h"

namespace humongousexplorer::editor
{
	struct ArchiveData;
}
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
		void GetOnArchiveAdded(std::unique_ptr<editor::ArchiveData>& a_pArchiveData);

		FileEntryView* m_pFilterFileEntryView = nullptr; // TODO: move
		SearchBar m_SearchBar;
	};
}