#pragma once

#include <string>
#include <vector>
#include <memory>

#include "core/System.h"
#include "core/Data.h"
#include "core/Event.h"
#include "core/Observable.h"

#include "file/FILEPCH.h"

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
namespace humongousexplorer::resources
{
	class Resource;
}
namespace humongousexplorer::editor
{
	//---------------------------------------------------------------------
	struct ArchiveData
	{
		fs::path m_sPath;
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
		const std::vector<std::unique_ptr<ArchiveData>>& GetArchives() const;
		void ClearArchives()
		{
			m_aArchives.clear();
		}

		void SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView);
		imgui::TreeFileEntryView* GetSelectedView();
		const core::Observable<imgui::TreeFileEntryView*>& GetSelectedViewObs() const;

		void SetSelectedResource(resources::Resource* a_pSelectedResource);
		resources::Resource* GetSelectedResource();
		const core::Observable<resources::Resource*>& GetSelectedResource() const;

		const core::SimpleEvent<const std::string&, const std::string&>& GetOnLoadArchiveFailed() const;
		const core::SimpleEvent<const std::string&>& GetOnLoadArchiveSuccess() const;
		const core::SimpleEvent<float>& GetOnLoadArchiveProgressed() const;

		const core::SimpleEvent<std::unique_ptr<ArchiveData>&>& GetOnArchiveAdded() const;
	private:
		resources::ResourceType m_ResourceTypeFilter;
		std::string m_sAppDataPath;
		std::vector<std::unique_ptr<ArchiveData>> m_aArchives;

		core::Observable<imgui::TreeFileEntryView*> m_pSelectedFileEntryView{nullptr};
		core::Observable<resources::Resource*> m_pSelectedResource{nullptr};

		core::SimpleEvent<const std::string&, const std::string&> m_evntOnLoadArchiveFailed;
		core::SimpleEvent<const std::string&> m_evntOnLoadArchiveSuccess;
		core::SimpleEvent<float> m_evntOnLoadArchiveProgressed;

		core::SimpleEvent<std::unique_ptr<ArchiveData>&> m_evntOnArchiveAdded;
	};
}