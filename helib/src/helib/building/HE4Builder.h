#pragma once

// external
#include <vector>

#include <helib/building/BuilderBase.h>

namespace humongousexplorer::archive
{
	class Archive;
}
namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::building
{
	struct SGENEntry
	{
		parsing::Chunk* m_pSGENChunk = nullptr;
		parsing::Chunk* m_pDIGIChunk = nullptr;
	};

	//---------------------------------------------------------------------
	// HE4Builder
	//---------------------------------------------------------------------
	/// <summary>
	/// Rebuilds a HE4 with the associated data.
	/// </summary>
	class HE4Builder : public BuilderBase
	{
	public:
		/// <summary>
		/// Associates the chunks before rebuilding other archive this archive is dependent on.
		/// </summary>
		bool Bind(archive::ArchiveSet& a_ArchiveSet) override;

		/// <summary>
		/// Builds the chunks in the associated archive.
		/// </summary>
		bool Build() override;
	protected:
		archive::Archive* m_pHE4 = nullptr;
		std::vector<SGENEntry> m_aSGENs;
	};
}