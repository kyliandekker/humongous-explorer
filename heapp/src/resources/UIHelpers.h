#pragma once

#include <string>
#include <unordered_map>

namespace humongousexplorer::archive
{
	enum class ArchiveType;
}
namespace humongousexplorer::resources
{
	enum class ResourceType;

	//---------------------------------------------------------------------
	// DisplayableChunk
	//---------------------------------------------------------------------
	struct DisplayableChunk
	{
		resources::ResourceType m_eResourceType;
		bool m_bVisible = true;
	};

	//---------------------------------------------------------------------
	std::string GetIconFromArchiveType(archive::ArchiveType a_ArchiveType);

	//---------------------------------------------------------------------
	const std::unordered_map<std::string_view, DisplayableChunk>& GetDisplayableChunks(archive::ArchiveType a_ArchiveType);
}
