#include "Workspace.h"

#include "win32/winfile.h"
#include "file/file.h"
#include "imgui/views/FileEntryView.h"
#include "resources/Resource.h"
#include "parsing/ChunkParser.h"
#include "core/Memory.h"
#include "parsing/ChunkIDs.h"

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
	const core::Event<>& Workspace::GetArchivesChanged() const
	{
		return m_onArchivesChanged;
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
	const core::Observable<resources::Resource*>& Workspace::GetSelectedResourceObs() const
	{
		return m_pSelectedResource;
	}

	//---------------------------------------------------------------------
	const archive::ArchiveSet& Workspace::GetArchiveSet() const
	{
		return m_ArchiveSet;
	}

	//---------------------------------------------------------------------
	archive::ArchiveSet& Workspace::GetArchiveSet()
	{
		return m_ArchiveSet;
	}
}
