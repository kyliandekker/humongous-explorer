#include "ResourceFileEntryView.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	ResourceFileEntryView::ResourceFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows) : FileEntryView(std::move(a_aRows))
	{
	}

	//---------------------------------------------------------------------
	TreeResourceFileEntryView::TreeResourceFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows) : TreeFileEntryView(std::move(a_aRows))
	{
	}
}