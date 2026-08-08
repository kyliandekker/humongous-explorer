#pragma once

#include "imgui/views/FileEntryView.h"

namespace humongousexplorer::resources
{
	enum class ResourceType;
}
namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	struct ResourceFileEntryView : public FileEntryView
	{
		ResourceFileEntryView(resources::ResourceType a_ResourceType, std::vector<std::unique_ptr<RowEntry>> a_aRows);

		resources::ResourceType m_ResourceType;
	};
}