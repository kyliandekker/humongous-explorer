#pragma once

#include <vector>
#include <memory>

#include "file/file.h"

namespace humongousexplorer::archive
{
	class Archive;

	//---------------------------------------------------------------------
	// Archive
	//---------------------------------------------------------------------
	/// <summary>
	/// Represents a Humongous Entertainment game with all archives, providing methods to load, store and inspect certain archives.
	/// </summary>
	class ArchiveSet
	{
	public:
		bool LoadArchives(const fs::path& a_Path);
		size_t GetHEVersion() const;
		size_t GetScriptVersion() const;
	private:
		void DetermineHEVersion();

		size_t m_iHEVersion = 0;
		size_t m_iScriptVersion = 0;
		std::vector<std::unique_ptr<Archive>> m_aArchives;
	};
}