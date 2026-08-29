#include "Archive.h"

#include <helib/archive/ArchiveType.h>
#include <helib/core/Data.h>
#include <helib/core/DataStream.h>
#include <helib/core/Log.h>
#include <helib/core/Memory.h>
#include <helib/file/file.h>
#include <helib/parsing/Chunk.h>
#include <helib/parsing/ChunkParser.h>

namespace humongousexplorer::archive
{
	//---------------------------------------------------------------------
	// Archive
	//---------------------------------------------------------------------
	bool Archive::Load(const fs::path& a_Path)
	{
		fs::path sanitizedPath = a_Path.lexically_normal();
		if (!fs::exists(sanitizedPath))
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + sanitizedPath.string() + "\": Path did not exist.");
			return false;
		}

		if (!fs::is_regular_file(sanitizedPath))
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + sanitizedPath.string() + "\": Path was not a file.");
			return false;
		}

		std::string extension = sanitizedPath.extension().string().substr(1);

		// Unknown or Folder.
		ArchiveType archiveType = archive::GetArchiveTypeFromExtension(extension);
		if (archiveType < archive::ArchiveType::HE0)
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + sanitizedPath.string() + "\": Unsupported archive type.");
			return false;
		}

		m_eType = archiveType;

		core::Data data;
		if (!file::LoadFile(sanitizedPath, data))
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + sanitizedPath.string() + "\": Could not read file.");
			return false;
		}

		if (data.empty())
		{
			core::Log(core::LogLevel::Error, "Failed to load: \"" + sanitizedPath.string() + "\": Archive file is empty.");
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
				core::Log(core::LogLevel::Error, "Failed to load: \"" + sanitizedPath.string() + "\": Failed to parse archive data.");
				return false;
			}
		}

		m_sName = sanitizedPath.filename().string();
		core::Log(core::LogLevel::Success, "Successfully loaded: \"" + sanitizedPath.string() + "\".");
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
