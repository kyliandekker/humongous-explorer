#pragma once

#include <vector>

namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::core
{
	class Data;
}
namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	enum class ResourceType;

	//---------------------------------------------------------------------
	class Resource
	{
	public:
		virtual void Initialize(parsing::Chunk* a_pChunk) = 0;
		virtual bool Replace(const core::Data& a_Data) = 0;

		ResourceType GetResourceType() const;
	protected:
		std::vector<parsing::Chunk*> m_aRelevantChunks;
		ResourceType m_eResourceType;
	};
}