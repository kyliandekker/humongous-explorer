#include "Archive.h"

#include "core/Data.h"
#include "core/DataStream.h"
#include "core/Memory.h"

#include "archive/ArchiveType.h"

#include "file/file.h"

#include "parsing/Chunk.h"
#include "parsing/ChunkParser.h"

namespace humongousexplorer::archive
{
	//---------------------------------------------------------------------
	// Archive
	//---------------------------------------------------------------------
	core::LoadResult Archive::Load(const fs::path& a_Path)
	{
		if (!fs::exists(a_Path))
		{
			return { core::LoadStatus::Failure, "Path did not exist." };
		}

		if (!fs::is_regular_file(a_Path))
		{
			return { core::LoadStatus::Failure, "Path was not a file." };
		}

		std::string extension = a_Path.extension().string().substr(1);

		// Unknown or Folder.
		ArchiveType archiveType = archive::GetArchiveTypeFromExtension(extension);
		if (archiveType < archive::ArchiveType::HE0)
		{
			return { core::LoadStatus::Failure, "Unsupported archive type." };
		}

		m_eType = archiveType;

		core::Data data;
		if (!file::LoadFile(a_Path, data))
		{
			return { core::LoadStatus::Failure, "Could not read file." };
		}

		if (data.empty())
		{
			return { core::LoadStatus::Failure, "Archive file is empty." };
		}

		m_pRoot = std::make_unique<parsing::Chunk>();
		if (!parsing::ParseArchive(*m_pRoot, data))
		{
			core::Data xorredDataContainer = data;
			unsigned char* xorredData = xorredDataContainer.dataAs<unsigned char>();

			m_pRoot->SetEncrypted(true);
			m_pRoot->SetEncryptionKey(0x69);

			core::xorShift(xorredData, xorredDataContainer.size(), m_pRoot->GetEncryptionKey());
			if (!parsing::ParseArchive(*m_pRoot, xorredDataContainer))
			{
				return { core::LoadStatus::Failure, "Failed to parse archive data." };
			}
		}

		m_sName = a_Path.filename().string();
		return { core::LoadStatus::Success, "" };
	}

	//---------------------------------------------------------------------
	void Archive::Build(core::DataStream& a_Data) const
	{
		a_Data = core::DataStream(m_pRoot->WholeChunkSize());
		m_pRoot->ToData(a_Data);

		if (m_pRoot->IsEncrypted())
		{
			unsigned char* data = a_Data.dataAs<unsigned char>();
			core::xorShift(data, a_Data.size(), m_pRoot->GetEncryptionKey());
		}
	}

	//---------------------------------------------------------------------
	const parsing::Chunk& Archive::GetRoot() const
	{
		return *m_pRoot.get();
	}

	//---------------------------------------------------------------------
	parsing::Chunk& Archive::GetRoot()
	{
		return *m_pRoot.get();
	}

	//---------------------------------------------------------------------
	ArchiveType Archive::GetType() const
	{
		return m_eType;
	}
}
