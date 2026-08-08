#include "ResourceFileEntryView.h"

namespace humongousexplorer::imgui
{
	ResourceFileEntryView::ResourceFileEntryView(resources::ResourceType a_ResourceType, std::vector<std::unique_ptr<RowEntry>> a_aRows) : FileEntryView(std::move(a_aRows)),
		m_ResourceType(a_ResourceType)
	{
	}
}