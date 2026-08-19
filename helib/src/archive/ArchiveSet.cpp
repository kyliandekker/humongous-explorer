#include "./ArchiveSet.h"

#include "archive/ArchiveType.h"
#include "archive/Archive.h"
#include "core/Log.h"

#include "parsing/Chunk.h"

#include "parsing/ChunkIDs.h"

namespace humongousexplorer::archive
{
	//---------------------------------------------------------------------
	// ArchiveSet
	//---------------------------------------------------------------------
	bool ArchiveSet::LoadArchives(const fs::path& a_Path)
	{
		m_aArchives.clear();
		m_aResults.clear();

		core::Log(core::LogLevel::Info, "Loading archives from: " + a_Path.filename().string() + ".");

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

		if (paths.empty())
		{
			core::Log(core::LogLevel::Warning, "No archive files found for: " + a_Path.stem().string() + ".");
			return false;
		}

		core::Log(core::LogLevel::Info, "Found " + std::to_string(paths.size()) + " archive(s)" + ".");

		for (const fs::path& filePath : paths)
		{
			std::unique_ptr<Archive> ptr = std::make_unique<Archive>();
			core::LoadResult result = ptr->Load(filePath);

			ArchiveLoadInfo info;
			info.path = filePath;
			info.result = result;
			m_aResults.push_back(info);

			std::string filename = filePath.filename().string();

			if (result.status == core::LoadStatus::Success)
			{
				core::Log(core::LogLevel::Success, "Loaded " + filename + ".");
				m_aArchives.push_back(std::move(ptr));
			}
			else
			{
				core::Log(core::LogLevel::Error, "Failed to load " + filename + ": " + result.errorMessage + ".");
			}
		}

		m_iScriptVersion = 6;
		DetermineHEVersion();

		core::Log(core::LogLevel::Info, "HE version: " + std::to_string(m_iHEVersion) + ", Script version: " + std::to_string(m_iScriptVersion) + ".");

		return !m_aArchives.empty();
	}

	//---------------------------------------------------------------------
	const std::vector<ArchiveLoadInfo>& ArchiveSet::GetResults() const
	{
		return m_aResults;
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