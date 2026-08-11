#pragma once

#include <string>
#include <vector>

#include "core/System.h"
#include "core/Data.h"
#include "parsing/HEParser.h"
#include "resources/ArchiveType.h"
#include "resources/ResourceType.h"

namespace humongousexplorer
{
	namespace editor
	{
		class Workspace;
	}

	extern editor::Workspace& GetWorkspace();
}
namespace humongousexplorer::imgui
{
	class TreeFileEntryView;
}
namespace humongousexplorer::editor
{
	struct ArchiveData
	{
		std::string m_sPath;
		resources::ArchiveType m_eType = resources::ArchiveType::Unknown;
		core::Data m_Data;
		parsing::Chunk m_Root;
	};

	class Workspace : public core::System
	{
	public:
		/// <summary>
		/// Initializes the system, setting up necessary resources.
		/// </summary>
		/// <returns>True if the initialization was successful, otherwise false.</returns>
		bool Initialize() override;

		resources::ResourceType GetResourceTypeFilter() const;
		void SetResourceTypeFilter(resources::ResourceType a_ResourceTypeFilter);
		const std::string& GetAppDataPath() const;

		void AddArchive(ArchiveData a_Archive);
		const std::vector<ArchiveData>& GetArchives() const;

		void SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView);
		imgui::TreeFileEntryView* GetSelectedView();
	private:
		resources::ResourceType m_ResourceTypeFilter;
		std::string m_sAppDataPath;
		std::vector<ArchiveData> m_aArchives;

		imgui::TreeFileEntryView* m_pSelectedFileEntryView = nullptr;
	};
}