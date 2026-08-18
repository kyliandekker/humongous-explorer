#pragma once

#include <memory>

#include "core/Data.h"

#include "file/FILEPCH.h"

namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::resources
{
	enum class ArchiveType;

	//---------------------------------------------------------------------
	class ArchiveEntry
	{
	public:
		ArchiveEntry(const fs::path& a_Path, ArchiveType a_eType);
		const fs::path& GetPath() const;

		ArchiveType GetType() const;

		const core::Data& GetData() const;
		core::Data& GetData();

		const parsing::Chunk& GetRoot() const;
		parsing::Chunk& GetRoot();
	private:
		fs::path m_Path;
		ArchiveType m_eType;
		core::Data m_Data;
		std::unique_ptr<parsing::Chunk> m_pRoot;
	};
}