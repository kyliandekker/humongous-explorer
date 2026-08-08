#pragma once

#include "imgui/views/FileEntryView.h"
#include "resources/ResourceType.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	struct ResourceFileEntryView : public FileEntryView
	{
		ResourceFileEntryView(resources::ResourceType a_ResourceType, std::vector<std::unique_ptr<RowEntry>> a_aRows);

		resources::ResourceType m_ResourceType;
	};

	//---------------------------------------------------------------------
	inline std::unique_ptr<ResourceFileEntryView> MakeResourceEntryView(
		resources::ResourceType a_eType,
		const std::string& a_sCount = "UNINITIALIZED"
	)
	{
		return std::make_unique<ResourceFileEntryView>(
			a_eType,
			MakeRows(
				MakeIconRow(resources::GetIconFromResourceType(a_eType)),
				MakeNameRow(resources::GetNameFromResourceType(a_eType)),
				MakeCountRow(a_sCount)
			)
		);
	}
}