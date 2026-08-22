#pragma once

#include <vector>
#include <memory>

#include "archive/Archive.h"
#include "core/LoadResult.h"
#include "file/file.h"

namespace humongousexplorer::archive
{
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

		std::vector<std::unique_ptr<Archive>>& GetArchives()
		{
			return m_aArchives;
		}

		const std::vector<std::unique_ptr<Archive>>& GetArchives() const
		{
			return m_aArchives;
		}
	private:
		void DetermineHEVersion();

		size_t m_iHEVersion = 0;
		size_t m_iScriptVersion = 0;
		std::vector<std::unique_ptr<Archive>> m_aArchives;
	};
}
