#include "./ArchiveSet.h"

#include "archive/ArchiveType.h"
#include "archive/Archive.h"

#include "parsing/Chunk.h"

#include "parsing/ChunkIDs.h"

namespace humongousexplorer::archive
{
	//---------------------------------------------------------------------
	// ArchiveSet
	//---------------------------------------------------------------------
	bool ArchiveSet::LoadArchives(const fs::path& a_Path)
	{
		std::vector<fs::path> paths;

		fs::path folder = a_Path.parent_path();

		for (const auto& entry : fs::directory_iterator(folder))
		{
			if (!entry.is_regular_file())
			{
				continue;
			}

			const fs::path& filePath = entry.path();

			std::string extension = filePath.extension().string().substr(1);
			if (GetArchiveTypeFromExtension(extension) < ArchiveType::HE0)
			{
				continue;
			}

			// Check if the filename is the same, not the extension.
			if (filePath.stem().generic_string() != a_Path.stem().generic_string())
			{
				continue;
			}

			paths.push_back(filePath);
		}

		for (const fs::path& filePath : paths)
		{
			std::unique_ptr<Archive> ptr = std::make_unique<Archive>();
			ptr->Load(filePath);
		}

		m_iScriptVersion = 6;
		DetermineHEVersion();

		return !m_aArchives.empty();
	}

	//---------------------------------------------------------------------
	size_t ArchiveSet::GetHEVersion() const
	{
		return m_iHEVersion;
	}

	//---------------------------------------------------------------------
	size_t ArchiveSet::GetScriptVersion() const
	{
		return m_iScriptVersion;
	}

	//---------------------------------------------------------------------
	void ArchiveSet::DetermineHEVersion()
	{
		Archive* he0 = nullptr;
		for (const std::unique_ptr<Archive>& archiveEntry : m_aArchives)
		{
			if (archiveEntry->GetType() == ArchiveType::HE0)
			{
				he0 = archiveEntry.get();
			}
		}

		if (!he0)
		{
			return;
		}

		parsing::Chunk* maxs = he0->GetRoot().TryFindChild(parsing::MAXS_CHUNK_ID);
		if (!maxs)
		{
			return;
		}

		m_iScriptVersion = 6;

		switch (maxs->WholeChunkSize())
		{
			case 52:
			{
				m_iHEVersion = 99;
				break;
			}
			case 46:
			{
				m_iHEVersion = 90;
				break;
			}
			case 40:
			{
				m_iHEVersion = 80;
				break;
			}
			case 38:
			{
				m_iHEVersion = 71;
				break;
			}
		}

		if (he0->GetRoot().TryFindChild(parsing::INIB_CHUNK_ID))
		{
			if (m_iHEVersion >= 90)
			{
				m_iHEVersion = 98;
			}
		}
		else if (m_iHEVersion < 72 && he0->GetRoot().TryFindChild(parsing::DROO_CHUNK_ID))
		{
			m_iHEVersion = 60;
		}
	}
}