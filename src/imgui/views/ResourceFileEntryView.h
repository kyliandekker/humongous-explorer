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
		ResourceFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows);
	};

	//---------------------------------------------------------------------
	struct TreeResourceFileEntryView : public TreeFileEntryView
	{
		TreeResourceFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows);
	};
}