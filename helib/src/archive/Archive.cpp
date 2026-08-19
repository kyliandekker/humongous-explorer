#include "./Archive.h"

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
	bool Archive::Load(const fs::path& a_Path)
	{
		std::string extension = a_Path.extension().string().substr(1);

		// Unknown or Folder.
		if (archive::GetArchiveTypeFromExtension(extension) < archive::ArchiveType::HE0)
		{
			return false;
		}

		core::Data data;
		if (!file::LoadFile(a_Path, data))
		{
			return false;
		}

		if (data.empty())
		{
			return false;
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
				return false; // Failed to parse file completely.
			}
		}
		return true;
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

}