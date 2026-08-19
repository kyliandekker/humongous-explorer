#pragma once

#include <memory>

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
	enum class ArchiveType;

	class Archive
	{
	public:
		bool Load(const fs::path& a_Path);
		void Build(core::DataStream& a_Data) const;

		const parsing::Chunk& GetRoot() const;
		parsing::Chunk& GetRoot();
	private:
		ArchiveType m_eType;
		std::unique_ptr<parsing::Chunk> m_pRoot;
	};
}