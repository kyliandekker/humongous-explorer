#pragma once

// external
#include <cstdint>
#include <string_view>
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
	using IdToChunk = std::vector<parsing::Chunk*>;

	//---------------------------------------------------------------------
	// HE0Builder
	//---------------------------------------------------------------------
	/// <summary>
	/// Rebuilds a HE0 with the associated data.
	/// </summary>
	class HE0Builder : public BuilderBase
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
		bool ParseDLFL(parsing::Chunk* a_pChunk, std::vector<uint32_t>& a_aOut);

		bool ReadDIRChunk(parsing::Chunk* a_pChunk, const std::string_view& a_sChunkID, uint16_t& a_iNumEntries, std::vector<uint8_t>& a_aIds, std::vector<uint32_t>& a_aOffsets, std::vector<uint32_t>& a_aSizes);

		bool BuildIdToChunk(parsing::Chunk* a_pDirChunk, archive::Archive& a_OriginalArchive, std::vector<uint32_t> const& a_aOriginalDLFLData, std::string_view a_sDirTag, IdToChunk& a_aOut);

		bool RebuildDLFL(parsing::Chunk* a_pChunk, archive::Archive& a_Archive);

		bool RebuildDIRChunk(parsing::Chunk* a_pChunk, std::vector<parsing::Chunk*> const& a_aIdToChunk, std::vector<uint32_t> const& a_aNewDLFLData, std::string_view a_sChunkID);

		archive::Archive* m_pHE0 = nullptr;
		archive::Archive* m_pA = nullptr;

		std::vector<uint32_t> m_aOriginalDLFLData;

		parsing::Chunk* m_pDLFLChunk = nullptr;

		parsing::Chunk* m_pDIRRChunk = nullptr;
		IdToChunk m_aDIRR; // RMDA byte offsets + 16.

		parsing::Chunk* m_pDIRSChunk = nullptr;
		IdToChunk m_aDIRS; // SCRP byte offsets + 16 (LSCR, LSC2, EXCD, ENCD are inside the RMDA).

		parsing::Chunk* m_pDIRNChunk = nullptr;
		IdToChunk m_aDIRN; // SOUN/DIGI/TALK byte offsets + 16 (only TALK inside the (A) numEntries).

		parsing::Chunk* m_pDIRCChunk = nullptr;
		IdToChunk m_aDIRC; // COST/AKOS byte offsets + 16 (Costume/Actor).

		parsing::Chunk* m_pDIRFChunk = nullptr;
		IdToChunk m_aDIRF; // CHAR byte offsets + 16 (Characters).

		parsing::Chunk* m_pDIRMChunk = nullptr;
		IdToChunk m_aDIRM; // (MULT/AWIZ) byte offsets + 16 (Movie/Animation).

		parsing::Chunk* m_pDIRTChunk = nullptr;
		IdToChunk m_aDIRT; // TLKE byte offsets + 16 (old TALKs from non-humongous games (afaik)).
	};
}