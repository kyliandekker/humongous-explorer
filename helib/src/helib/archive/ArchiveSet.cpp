#include "ArchiveSet.h"

#include <cctype>
#include <helib/archive/Archive.h>
#include <helib/archive/ArchiveType.h>
#include <helib/core/Log.h>
#include <helib/parsing/Chunk.h>
#include <helib/parsing/ChunkIDs.h>

namespace humongousexplorer::archive
{
	namespace
	{
		bool EqualsIgnoreCase(const std::string& a, const std::string& b)
		{
			if (a.size() != b.size())
			{
				return false;
			}
			for (size_t i = 0; i < a.size(); ++i)
			{
				if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i])))
				{
					return false;
				}
			}
			return true;
		}
	}

	//---------------------------------------------------------------------
	// ArchiveSet
	//---------------------------------------------------------------------
	bool ArchiveSet::LoadArchives(const fs::path& a_Path)
	{
		fs::path sanitizedPath = a_Path.lexically_normal();
		m_aArchives.clear();

		core::Log(core::LogLevel::Info, "Loading archives from: " + sanitizedPath.filename().string() + ".");

		std::vector<fs::path> paths;

		fs::path folder = sanitizedPath.parent_path();

		std::error_code ec;
		fs::directory_iterator it(folder, ec);
		if (ec)
		{
			core::Log(core::LogLevel::Warning, "Failed to iterate directory: " + folder.string() + ".");
			return false;
		}
		for (const auto& entry : it)
		{
			if (!entry.is_regular_file())
			{
				continue;
			}

			const fs::path& filePath = entry.path();

			std::string extension = filePath.extension().string();
			extension = extension.empty() ? "" : extension.substr(1);
			if (GetArchiveTypeFromExtension(extension) < ArchiveType::HE0)
			{
				continue;
			}

			// Check if the filename is the same, not the extension (case-insensitive for Windows compatibility).
			if (!EqualsIgnoreCase(filePath.stem().string(), sanitizedPath.stem().string()))
			{
				continue;
			}

			paths.push_back(filePath);
		}

		if (paths.empty())
		{
			core::Log(core::LogLevel::Warning, "No archive files found for: " + sanitizedPath.stem().string() + ".");
			return false;
		}

		core::Log(core::LogLevel::Info, "Found " + std::to_string(paths.size()) + " archive(s)" + ".");

		for (const fs::path& filePath : paths)
		{
			std::unique_ptr<Archive> ptr = std::make_unique<Archive>();
			std::string filename = filePath.filename().string();
			if (!ptr->Load(filePath))
			{
				continue;
			}
			m_aArchives.push_back(std::move(ptr));
		}

		m_iScriptVersion = 6;
		DetermineHEVersion();

		core::Log(core::LogLevel::Info, "HE version: " + std::to_string(m_iHEVersion) + ", Script version: " + std::to_string(m_iScriptVersion) + ".");

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
				break;
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