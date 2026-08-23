#include "Archive.h"

#include "core/Data.h"
#include "core/DataStream.h"
#include "core/Memory.h"
#include "core/Log.h"

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
		if (!fs::exists(a_Path))
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + a_Path.string() + "\": Path did not exist.");
			return false;
		}

		if (!fs::is_regular_file(a_Path))
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + a_Path.string() + "\": Path was not a file.");
			return false;
		}

		std::string extension = a_Path.extension().string().substr(1);

		// Unknown or Folder.
		ArchiveType archiveType = archive::GetArchiveTypeFromExtension(extension);
		if (archiveType < archive::ArchiveType::HE0)
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + a_Path.string() + "\": Unsupported archive type.");
			return false;
		}

		m_eType = archiveType;

		core::Data data;
		if (!file::LoadFile(a_Path, data))
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + a_Path.string() + "\": Could not read file.");
			return false;
		}

		if (data.empty())
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + a_Path.string() + "\": Archive file is empty.");
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
				core::Log(core::LogLevel::Error, "Failed to load: \"" + a_Path.string() + "\": Failed to parse archive data.");
				return false;
			}
		}

		m_sName = a_Path.filename().string();
		core::Log(core::LogLevel::Success, "Successfully loaded: \"" + a_Path.string() + "\".");
		return true;
	}

	//---------------------------------------------------------------------
	void Archive::Build(core::DataStream& a_Data, bool a_bEncrypt) const
	{
		if (!m_pRoot)
		{
			a_Data.Free();
			return;
		}

		const size_t size = m_pRoot->ChunkSize();
		if (size == 0)
		{
			a_Data.Free();
			return;
		}

		a_Data = core::DataStream(size);
		m_pRoot->ToData(a_Data);

		if (a_bEncrypt && m_pRoot->IsEncrypted())
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
