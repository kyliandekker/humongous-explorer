#include "ArchiveEntry.h"

#include "parsing/Chunk.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	// ArchiveEntry
	//---------------------------------------------------------------------
	ArchiveEntry::ArchiveEntry(const fs::path& a_Path, ArchiveType a_eType) :
		m_Path(a_Path),
		m_eType(a_eType)
	{
		m_pRoot = std::make_unique<parsing::Chunk>();
	}

	//---------------------------------------------------------------------
	const fs::path& ArchiveEntry::GetPath() const
	{
		return m_Path;
	}

	//---------------------------------------------------------------------
	ArchiveType ArchiveEntry::GetType() const
	{
		return m_eType;
	}

	//---------------------------------------------------------------------
	const core::Data& ArchiveEntry::GetData() const
	{
		return m_Data;
	}

	//---------------------------------------------------------------------
	core::Data& ArchiveEntry::GetData()
	{
		return m_Data;
	}

	//---------------------------------------------------------------------
	const parsing::Chunk& ArchiveEntry::GetRoot() const
	{
		return *m_pRoot.get();
	}

	//---------------------------------------------------------------------
	parsing::Chunk& ArchiveEntry::GetRoot()
	{
		return *m_pRoot.get();
	}
}