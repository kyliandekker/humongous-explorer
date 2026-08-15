#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "core/System.h"
#include "core/Data.h"
#include "core/Event.h"
#include "core/Observable.h"

#include "file/FILEPCH.h"

#include "parsing/Chunk.h"

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
	class ArchiveEntry;
	class Resource;
}
namespace humongousexplorer::editor
{
	//---------------------------------------------------------------------
	class Workspace : public core::System
	{
	public:
		bool Initialize() override;

		resources::ResourceType GetResourceTypeFilter() const;
		void SetResourceTypeFilter(resources::ResourceType a_ResourceTypeFilter);
		const std::string& GetAppDataPath() const;

		void LoadArchives(const fs::path& a_sPath);
		const std::vector<std::unique_ptr<resources::ArchiveEntry>>& GetArchives() const;

		const core::Event<>& GetArchivesChanged() const;
		const core::Event<bool, const std::string&>& GetStatusMessageUpdated() const;

		void SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView);
		imgui::TreeFileEntryView* GetSelectedView();
		const core::Observable<imgui::TreeFileEntryView*>& GetSelectedViewObs() const;

		void SetSelectedResource(resources::Resource* a_pSelectedResource);
		resources::Resource* GetSelectedResource();
		const core::Observable<resources::Resource*>& GetSelectedResource() const;
	private:
		resources::ResourceType m_ResourceTypeFilter;
		std::string m_sAppDataPath;
		std::vector<std::unique_ptr<resources::ArchiveEntry>> m_aArchives;

		core::Event<> m_onArchivesChanged;
		core::Event<bool, const std::string&> m_onStatusMessageUpdated;

		core::Observable<imgui::TreeFileEntryView*> m_pSelectedFileEntryView{nullptr};
		core::Observable<resources::Resource*> m_pSelectedResource{nullptr};
	};
}