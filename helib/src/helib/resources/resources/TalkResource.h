#pragma once

#include <helib/resources/Resource.h>

namespace humongousexplorer::archive
{
	class Archive;
}
namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	// TalkResource
	//---------------------------------------------------------------------
	class TalkResource : public Resource
	{
	public:
		archive::Archive* m_pHE0 = nullptr;
		archive::Archive* m_pA = nullptr;

		void Initialize(parsing::Chunk* a_pChunk) override;
		bool Replace(const core::Data& a_Data) override;
	};
}