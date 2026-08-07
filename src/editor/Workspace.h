#pragma once

#include <string>

#include "core/System.h"

namespace humongousexplorer
{
	namespace editor
	{
		class Workspace;
	}

	extern editor::Workspace& GetWorkspace();
}
namespace humongousexplorer::resources
{
	enum class ResourceType;
}
namespace humongousexplorer::editor
{
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
	private:
		resources::ResourceType m_ResourceTypeFilter;
		std::string m_sAppDataPath;
	};
}