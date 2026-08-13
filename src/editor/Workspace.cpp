#include "Workspace.h"

#include "file/file_abstractions.h"

#include "imgui/views/FileEntryView.h"

#include "resources/Resource.h"

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
	ArchiveData& Workspace::AddArchive(ArchiveData a_Archive)
	{
		m_aArchives.push_back(std::make_unique<ArchiveData>(std::move(a_Archive)));
		std::unique_ptr<ArchiveData>& archiveData = m_aArchives.back();
		archiveData->m_Root.FixParents(archiveData->m_Root);

		m_evntOnArchiveAdded(*archiveData);
		return *archiveData;
	}

	//---------------------------------------------------------------------
	const std::vector<std::unique_ptr<ArchiveData>>& Workspace::GetArchives() const
	{
		return m_aArchives;
	}

	//---------------------------------------------------------------------
	void Workspace::SetSelectedFileEntryView(imgui::TreeFileEntryView* a_pSelectedView)
	{
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

	//---------------------------------------------------------------------
	const core::SimpleEvent<const std::string&, const std::string&>& Workspace::GetOnLoadArchiveFailed() const
	{
		return m_evntOnLoadArchiveFailed;
	}

	//---------------------------------------------------------------------
	const core::SimpleEvent<const std::string&>& Workspace::GetOnLoadArchiveSuccess() const
	{
		return m_evntOnLoadArchiveSuccess;
	}

	//---------------------------------------------------------------------
	const core::SimpleEvent<float>& Workspace::GetOnLoadArchiveProgressed() const
	{
		return m_evntOnLoadArchiveProgressed;
	}

	//---------------------------------------------------------------------
	const core::SimpleEvent<ArchiveData&>& Workspace::GetOnArchiveAdded() const
	{
		return m_evntOnArchiveAdded;
	}
}