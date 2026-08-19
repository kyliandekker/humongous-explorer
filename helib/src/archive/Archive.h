#pragma once

#include <memory>

#include "archive/ArchiveType.h"
#include "file/FILEPCH.h"

namespace humongousexplorer::parsing
{
	class Chunk;
}

namespace humongousexplorer::core
{
	class DataStream;
}
namespace humongousexplorer::archive
{
	//---------------------------------------------------------------------
	// Archive
	//---------------------------------------------------------------------
	/// <summary>
	/// Represents a Humongous Entertainment archive file, providing methods to load, build, and inspect the chunk hierarchy.
	/// </summary>
	class Archive
	{
	public:
		/// <summary>
		/// Loads an archive file from disk and parses its chunk hierarchy.
		/// </summary>
		/// <param name="a_Path">The file path of the archive to load.</param>
		/// <returns>True if the archive was loaded and parsed successfully, otherwise false.</returns>
		bool Load(const fs::path& a_Path);

		/// <summary>
		/// Rebuilds the archive into a DataStream, re-serializing the chunk hierarchy.
		/// </summary>
		/// <param name="a_Data">The output DataStream containing the rebuilt archive data.</param>
		void Build(core::DataStream& a_Data) const;

		/// <summary>
		/// Retrieves the root chunk of the archive (read-only).
		/// </summary>
		/// <returns>A const reference to the root chunk.</returns>
		const parsing::Chunk& GetRoot() const;

		/// <summary>
		/// Retrieves the root chunk of the archive (modifiable).
		/// </summary>
		/// <returns>A reference to the root chunk.</returns>
		parsing::Chunk& GetRoot();

		/// <summary>
		/// Retrieves the archive type determined from the file extension.
		/// </summary>
		/// <returns>The ArchiveType of this archive.</returns>
		ArchiveType GetType() const;
	private:
		ArchiveType m_eType = ArchiveType::Unknown;
		std::unique_ptr<parsing::Chunk> m_pRoot;
	};
}