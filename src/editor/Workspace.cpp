#include "Workspace.h"

#include "file/file_abstractions.h"
#include "imgui/views/FileEntryView.h"
#include "resources/Resource.h"
#include "resources/ArchiveType.h"
#include "parsing/ChunkParser.h"
#include "core/Memory.h"
#include "logger/Logger.h"
#include "resources/ArchiveEntry.h"

namespace humongousexplorer
{
	//---------------------------------------------------------------------
	editor::Workspace& GetWorkspace()
	{
		static humongousexplorer::editor::Workspace workspace;

		return workspace;
	}
}

namespace humongousexplorer::editor
{
	//---------------------------------------------------------------------
	bool Workspace::Initialize()
	{
		m_sAppDataPath = file::GetAppDataPath().generic_string() + "/humongous_explorer";
		file::CreateDirectory(m_sAppDataPath);
		return core::System::Initialize();
	}

	//---------------------------------------------------------------------
	resources::ResourceType Workspace::GetResourceTypeFilter() const
	{
		return m_ResourceTypeFilter;
	}

	//---------------------------------------------------------------------
	void Workspace::SetResourceTypeFilter(resources::ResourceType a_ResourceTypeFilter)
	{
		m_ResourceTypeFilter = a_ResourceTypeFilter;
	}

	//---------------------------------------------------------------------
	const std::string& Workspace::GetAppDataPath() const
	{
		return m_sAppDataPath;
	}

	//---------------------------------------------------------------------
	void Workspace::LoadArchives(const fs::path& a_sPath)
	{
		m_aArchives.clear();
		m_pSelectedResource = nullptr;
		m_pSelectedFileEntryView = nullptr;

		fs::path folder = a_sPath.parent_path();

		std::vector<fs::path> paths;

		for (const auto& entry : fs::directory_iterator(folder))
		{
			if (!entry.is_regular_file())
			{
				continue;
			}

			const fs::path& filePath = entry.path();

			std::string extension = filePath.extension().string().substr(1);

			if (resources::GetArchiveTypeFromExtension(extension) < resources::ArchiveType::HE0)
			{
				continue;
			}

			if (filePath.stem().generic_string() != a_sPath.stem().generic_string())
			{
				continue;
			}

			paths.push_back(filePath);
		}

		for (const fs::path& filePath : paths)
		{
			auto ptr = std::make_unique<resources::ArchiveEntry>(
				filePath,
				resources::GetArchiveTypeFromExtension(filePath.extension().string().substr(1))
			);
			if (!file::LoadFile(filePath, ptr->GetData()))
			{
				LOGF(
					LogSeverity::LOGSEVERITY_ERROR,
					"Could not load archive: \"%s\"",
					filePath.filename().generic_string().c_str()
				);
				continue;
			}

			if (!ptr->GetData().empty())
			{
				if (!parsing::ParseArchive(ptr->GetRoot(), ptr->GetData()))
				{
					core::Data xorredData = ptr->GetData();

					unsigned char* data = xorredData.dataAs<unsigned char>();

					ptr->GetRoot().SetEncrypted(true);
					ptr->GetRoot().SetEncryptionKey(0x69);

					core::xorShift(data, xorredData.size(), ptr->GetRoot().GetEncryptionKey());

					if (!parsing::ParseArchive(ptr->GetRoot(), xorredData))
					{
						LOGF(
							LogSeverity::LOGSEVERITY_ERROR,
							"Could not parse archive: \"%s\"",
							filePath.filename().generic_string().c_str()
						);
						continue;
					}
				}
			}

			m_aArchives.push_back(std::move(ptr));
		}

		DetermineScriptAndHEVersion();

		m_onStatusMessageUpdated(!m_aArchives.empty(), !m_aArchives.empty() ? "Successfully loaded archives." : "Failed to load archives.");

		m_onArchivesChanged();
	}

	//---------------------------------------------------------------------
	void Workspace::DetermineScriptAndHEVersion()
	{
		resources::ArchiveEntry* he0 = nullptr;
		for (const std::unique_ptr<resources::ArchiveEntry>& archiveEntry : m_aArchives)
		{
			if (archiveEntry->GetType() == resources::ArchiveType::HE0)
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

		printf("Test");
	}

	//---------------------------------------------------------------------
	const std::vector<std::unique_ptr<resources::ArchiveEntry>>& Workspace::GetArchives() const
	{
		return m_aArchives;
	}

	//---------------------------------------------------------------------
	const core::Event<>& Workspace::GetArchivesChanged() const
	{
		return m_onArchivesChanged;
	}

	//---------------------------------------------------------------------
	const core::Event<bool, const std::string&>& Workspace::GetStatusMessageUpdated() const
	{
		return m_onStatusMessageUpdated;
	}

	//---------------------------------------------------------------------
	void Workspace::SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView)
	{
		if (m_pSelectedFileEntryView == a_pSelectedView)
		{
			return;
		}

		m_pSelectedFileEntryView = a_pSelectedView;
	}

	//---------------------------------------------------------------------
	imgui::TreeFileEntryView* Workspace::GetSelectedView()
	{
		return m_pSelectedFileEntryView.get();
	}

	//---------------------------------------------------------------------
	const core::Observable<imgui::TreeFileEntryView*>& Workspace::GetSelectedViewObs() const
	{
		return m_pSelectedFileEntryView;
	}

	//---------------------------------------------------------------------
	void Workspace::SetSelectedResource(resources::Resource* a_pSelectedResource)
	{
		m_pSelectedResource = a_pSelectedResource;
	}

	//---------------------------------------------------------------------
	resources::Resource* Workspace::GetSelectedResource()
	{
		return m_pSelectedResource.get();
	}

	//---------------------------------------------------------------------
	const core::Observable<resources::Resource*>& Workspace::GetSelectedResource() const
	{
		return m_pSelectedResource;
	}
}
