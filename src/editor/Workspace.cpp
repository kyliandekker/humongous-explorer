#include "Workspace.h"

#include "file/file_abstractions.h"

namespace humongousexplorer
{
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
}