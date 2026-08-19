#pragma once

#include <vector>
#include <memory>

#include "archive/Archive.h"
#include "core/LoadResult.h"
#include "file/file.h"

namespace humongousexplorer::archive
{
	//---------------------------------------------------------------------
	// ArchiveLoadInfo
	//---------------------------------------------------------------------
	/// <summary>
	/// Captures the result of loading a single archive file, including its path and outcome.
	/// </summary>
	struct ArchiveLoadInfo
	{
		fs::path path;
		core::LoadResult result;
	};

	//---------------------------------------------------------------------
	// ArchiveSet
	//---------------------------------------------------------------------
	/// <summary>
	/// Represents a Humongous Entertainment game with all archives, providing methods to load, store and inspect certain archives.
	/// </summary>
	class ArchiveSet
	{
	public:
		/// <summary>
		/// Loads all archive files sharing the same stem as the given path, logging each result to stdout.
		/// </summary>
		/// <param name="a_Path">Any archive file belonging to the game to load.</param>
		/// <returns>True if at least one archive loaded successfully, otherwise false.</returns>
		bool LoadArchives(const fs::path& a_Path);

		/// <summary>
		/// Retrieves the per-archive load results from the last LoadArchives call.
		/// </summary>
		/// <returns>A const reference to the vector of per-archive load results.</returns>
		const std::vector<ArchiveLoadInfo>& GetResults() const;

		/// <summary>
		/// Retrieves the detected Humongous Entertainment version.
		/// </summary>
		/// <returns>The HE version number.</returns>
		size_t GetHEVersion() const;

		/// <summary>
		/// Retrieves the detected script version.
		/// </summary>
		/// <returns>The script version number.</returns>
		size_t GetScriptVersion() const;

		Archive* GetArchive(size_t a_iIndex)
		{
			return m_aArchives[a_iIndex].get();
		}
	private:
		void DetermineHEVersion();

		size_t m_iHEVersion = 0;
		size_t m_iScriptVersion = 0;
		std::vector<std::unique_ptr<Archive>> m_aArchives;
		std::vector<ArchiveLoadInfo> m_aResults;
	};
}
