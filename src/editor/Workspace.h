#pragma once

#include <string>
#include <vector>

#include "core/System.h"
#include "core/Data.h"
#include "core/Observable.h"

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
	//---------------------------------------------------------------------
	struct ArchiveData
	{
		std::string m_sPath;
		resources::ArchiveType m_eType = resources::ArchiveType::Unknown;
		core::Data m_Data;
		parsing::Chunk m_Root;
	};

	//---------------------------------------------------------------------
	class Workspace : public core::System
	{
	public:
		bool Initialize() override;

		resources::ResourceType GetResourceTypeFilter() const;
		void SetResourceTypeFilter(resources::ResourceType a_ResourceTypeFilter);
		const std::string& GetAppDataPath() const;

		ArchiveData& AddArchive(ArchiveData a_Archive);
		const std::vector<ArchiveData>& GetArchives() const;

		void SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView);
		imgui::TreeFileEntryView* GetSelectedView();
		const core::Observable<imgui::TreeFileEntryView*>& GetSelectedViewObs() const;
	private:
		resources::ResourceType m_ResourceTypeFilter;
		std::string m_sAppDataPath;
		std::vector<ArchiveData> m_aArchives;

		core::Observable<imgui::TreeFileEntryView*> m_pSelectedFileEntryView{nullptr};
	};
}