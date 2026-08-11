#include "Workspace.h"

#include "file/file_abstractions.h"

#include "imgui/views/FileEntryView.h"

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
		m_aArchives.push_back(std::move(a_Archive));
		return m_aArchives.back();
	}

	//---------------------------------------------------------------------
	const std::vector<ArchiveData>& Workspace::GetArchives() const
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
}