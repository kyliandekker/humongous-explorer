#pragma once

// external
#include <vector>
#include <shtypes.h>

// file
#include <helib/file/FILEPCH.h>

#if defined(CreateDirectory)
#undef CreateDirectory
#undef CreateDirectoryA
#undef CreateDirectoryW
#endif

namespace humongousexplorer::file
{
	/// <summary>
	/// Opens a file open dialog.
	/// </summary>
	/// <param name="a_sPath">The file that was selected.</param>
	/// <param name="a_aFilters">List of file extensions to filter.</param>
	/// <returns>True if file was selected, false otherwise.</returns>
	bool PickFile(fs::path& a_sPath, const std::vector<COMDLG_FILTERSPEC>& a_aFilters = {}, const fs::path& a_InitialPath = {});

	/// <summary>
	/// Opens a file save dialog.
	/// </summary>
	/// <param name="a_sPath">The file that was saved.</param>
	/// <param name="a_aFilters">List of file extensions to filter.</param>
	/// <returns>True if file was saved, false otherwise.</returns>
	bool SaveFile(fs::path& a_sPath, const std::vector<COMDLG_FILTERSPEC>& a_aFilters = {}, const fs::path& a_InitialPath = {});

	/// <summary>
	/// Retrieves the app data path.
	/// </summary>
	/// <returns>Path to the app data folder.</returns>
	const fs::path GetAppDataPath();

	/// <summary>
	/// Opens the explorer in a specified path.
	/// </summary>
	/// <param name="a_sPath">The directory to open.</param>
	/// <returns>True if successful, false otherwise.</returns>
	bool OpenInExplorer(const fs::path& a_Path);
}