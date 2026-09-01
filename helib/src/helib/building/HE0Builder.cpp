#include "./HE0Builder.h"

// external
#include <cassert>

#include <helib/archive/Archive.h>
#include <helib/archive/ArchiveSet.h>
#include <helib/archive/ArchiveType.h>
#include <helib/core/DataStream.h>
#include <helib/core/Log.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/parsing/chunks/sound/SGEN_Chunk.h>

namespace humongousexplorer::building
{
	//---------------------------------------------------------------------
	// HE0Builder
	//---------------------------------------------------------------------
	bool HE0Builder::Bind(archive::ArchiveSet& a_ArchiveSet)
	{
		m_pHE0 = nullptr;
		m_pA = nullptr;
		m_aOriginalDLFLData.clear();
		m_pDLFLChunk = nullptr;
		m_pDIRRChunk = nullptr;
		m_aDIRR.clear();
		m_pDIRSChunk = nullptr;
		m_aDIRS.clear();
		m_pDIRNChunk = nullptr;
		m_aDIRN.clear();
		m_pDIRCChunk = nullptr;
		m_aDIRC.clear();
		m_pDIRFChunk = nullptr;
		m_aDIRF.clear();
		m_pDIRMChunk = nullptr;
		m_aDIRM.clear();
		m_pDIRTChunk = nullptr;
		m_aDIRT.clear();

		for (std::unique_ptr<archive::Archive>& archive : a_ArchiveSet.GetArchives())
		{
			if (archive->GetType() == archive::ArchiveType::HE0)
			{
				m_pHE0 = archive.get();
			}
			else if (archive->GetType() == archive::ArchiveType::A)
			{
				m_pA = archive.get();
			}
		}

		// Cache chunks in a map beforehand.
		std::unordered_map<std::size_t, parsing::Chunk*> dirChunkTable;
		std::vector<parsing::Chunk*> dirChunks;
		m_pHE0->GetRoot().TryFindChildren({
			parsing::DLFL_CHUNK_ID,
			parsing::DIRR_CHUNK_ID,
			parsing::DIRS_CHUNK_ID,
			parsing::DIRN_CHUNK_ID,
			parsing::DIRC_CHUNK_ID,
			parsing::DIRF_CHUNK_ID,
			parsing::DIRM_CHUNK_ID,
			parsing::DIRT_CHUNK_ID,
		}, dirChunks);
		for (parsing::Chunk* dirChunk : dirChunks)
		{
			if (dirChunk->GetTag() == parsing::DLFL_CHUNK_ID)
			{
				m_pDLFLChunk = dirChunk;
			}
			else if (dirChunk->GetTag() == parsing::DIRR_CHUNK_ID)
			{
				m_pDIRRChunk = dirChunk;
			}
			else if (dirChunk->GetTag() == parsing::DIRS_CHUNK_ID)
			{
				m_pDIRSChunk = dirChunk;
			}
			else if (dirChunk->GetTag() == parsing::DIRN_CHUNK_ID)
			{
				m_pDIRNChunk = dirChunk;
			}
			else if (dirChunk->GetTag() == parsing::DIRC_CHUNK_ID)
			{
				m_pDIRCChunk = dirChunk;
			}
			else if (dirChunk->GetTag() == parsing::DIRF_CHUNK_ID)
			{
				m_pDIRFChunk = dirChunk;
			}
			else if (dirChunk->GetTag() == parsing::DIRM_CHUNK_ID)
			{
				m_pDIRMChunk = dirChunk;
			}
			else if (dirChunk->GetTag() == parsing::DIRT_CHUNK_ID)
			{
				m_pDIRTChunk = dirChunk;
			}
		}

		assert(m_pHE0);
		if (!m_pHE0)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find HE0 archive.");
			return false;
		}

		assert(m_pA);
		if (!m_pA)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find (A) archive.");
			return false;
		}

		assert(m_pDLFLChunk);
		if (!m_pDLFLChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DLFL chunk in HE0 archive.");
			return false;
		}

		if (!ParseDLFL(m_pDLFLChunk, m_aOriginalDLFLData))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DLFL data.");
			return false;
		}

		assert(m_pDIRRChunk);
		if (!m_pDIRRChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DIRR chunk in HE0 archive.");
			return false;
		}

		if (!BuildIdToChunk(m_pDIRRChunk, *m_pA, m_aOriginalDLFLData, parsing::DIRR_CHUNK_ID, m_aDIRR))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DIRR chunk.");
			return false;
		}

		assert(m_pDIRSChunk);
		if (!m_pDIRSChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DIRS chunk in HE0 archive.");
			return false;
		}

		if (!BuildIdToChunk(m_pDIRSChunk, *m_pA, m_aOriginalDLFLData, parsing::DIRS_CHUNK_ID, m_aDIRS))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DIRS chunk.");
			return false;
		}

		assert(m_pDIRNChunk);
		if (!m_pDIRNChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DIRN chunk in HE0 archive.");
			return false;
		}

		if (!BuildIdToChunk(m_pDIRNChunk, *m_pA, m_aOriginalDLFLData, parsing::DIRN_CHUNK_ID, m_aDIRN))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DIRN chunk.");
			return false;
		}

		assert(m_pDIRCChunk);
		if (!m_pDIRCChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DIRC chunk in HE0 archive.");
			return false;
		}

		if (!BuildIdToChunk(m_pDIRCChunk, *m_pA, m_aOriginalDLFLData, parsing::DIRC_CHUNK_ID, m_aDIRC))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DIRC chunk.");
			return false;
		}

		assert(m_pDIRFChunk);
		if (!m_pDIRFChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DIRF chunk in HE0 archive.");
			return false;
		}

		if (!BuildIdToChunk(m_pDIRFChunk, *m_pA, m_aOriginalDLFLData, parsing::DIRF_CHUNK_ID, m_aDIRF))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DIRF chunk.");
			return false;
		}

		assert(m_pDIRMChunk);
		if (!m_pDIRMChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DIRM chunk in HE0 archive.");
			return false;
		}

		if (!BuildIdToChunk(m_pDIRMChunk, *m_pA, m_aOriginalDLFLData, parsing::DIRM_CHUNK_ID, m_aDIRM))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DIRM chunk.");
			return false;
		}

		// Generally, there will be no DIRT entries, but the chunk should still be present.
		assert(m_pDIRTChunk);
		if (!m_pDIRTChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not find DIRT chunk in HE0 archive.");
			return false;
		}

		if (!BuildIdToChunk(m_pDIRTChunk, *m_pA, m_aOriginalDLFLData, parsing::DIRT_CHUNK_ID, m_aDIRT))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE0: Could not parse DIRT chunk.");
			return false;
		}

		return true;
	}

	// DIR chunks are index chunks in HE0.
	// numChunks = uint16_t, number of chunks in the (a) + 1, LE.
	// roomIds = uint8_t * numChunks, LE.
	// offsets = uint32_t * numChunks, LE.
	// Here is a mental note:
	//std::unordered_map<std::string_view, std::vector<std::string_view>> chunks_in_index_pair =
	//{
	//	{ parsing::DIRR_CHUNK_ID, { parsing::RMDA_CHUNK_ID } },
	//	{ parsing::DIRS_CHUNK_ID, { parsing::SCRP_CHUNK_ID } },
	//	{ parsing::DIRN_CHUNK_ID, { parsing::SOUN_CHUNK_ID, parsing::TALK_CHUNK_ID, parsing::DIGI_CHUNK_ID } },
	//	{ parsing::DIRC_CHUNK_ID, { parsing::AKOS_CHUNK_ID } },
	//	{ parsing::DIRF_CHUNK_ID, { parsing::CHAR_CHUNK_ID } },
	//	{ parsing::DIRM_CHUNK_ID, { parsing::MULT_CHUNK_ID, parsing::AWIZ_CHUNK_ID } },
	//	{ parsing::DIRT_CHUNK_ID, { parsing::TLKE_CHUNK_ID } },
	//};
	//---------------------------------------------------------------------
	bool HE0Builder::ReadDIRChunk(parsing::Chunk* a_pChunk, const std::string_view& a_sChunkID, uint16_t& a_iNumEntries, std::vector<uint8_t>& a_aIds, std::vector<uint32_t>& a_aOffsets, std::vector<uint32_t>& a_aSizes)
	{
		// Reset
		a_iNumEntries = 0;
		a_aIds.clear();
		a_aOffsets.clear();
		a_aSizes.clear();

		std::string chunkId = std::string(a_sChunkID);

		assert(a_pChunk);
		if (!a_pChunk)
		{
			core::Log(core::LogLevel::Error, "Failed reading " + chunkId + ": Chunk was null.");
			return false;
		}

		assert(a_pChunk->GetTag() == a_sChunkID);
		if (a_pChunk->GetTag() != a_sChunkID)
		{
			core::Log(core::LogLevel::Error, "Failed reading " + chunkId + ": Chunk was not a " + chunkId + " chunk.");
			return false;
		}

		core::DataStream data = a_pChunk->GetData();
		data.ReadLE16(a_iNumEntries);
		for (size_t i = 0; i < static_cast<size_t>(a_iNumEntries); i++)
		{
			uint8_t id;
			data.Read(&id, sizeof(id), 1);
			a_aIds.push_back(id);
		}

		for (size_t i = 0; i < static_cast<size_t>(a_iNumEntries); i++)
		{
			uint32_t offset;
			data.ReadLE32(offset);
			a_aOffsets.push_back(offset);
		}

		size_t tell = data.Tell();
		size_t dataSize = a_pChunk->ChunkSize();

		// Some HE games have size also appended in the entries.
		if (tell < dataSize)
		{
			int64_t diff = static_cast<int64_t>(dataSize) - static_cast<int64_t>(tell);
			assert(diff == (static_cast<int64_t>(sizeof(uint32_t)) * a_iNumEntries)); // Double check it is exactly a size array.

			for (size_t i = 0; i < static_cast<size_t>(a_iNumEntries); i++)
			{
				uint32_t offset;
				data.ReadLE32(offset);
				a_aSizes.push_back(offset);
			}
		}

		tell = data.Tell();
		assert(tell == dataSize);
		if (tell != dataSize)
		{
			core::Log(core::LogLevel::Error, "Failed reading " + chunkId + ": Chunk is longer than expected. Is there dangling data?");
			return false;
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool HE0Builder::BuildIdToChunk(
		parsing::Chunk* a_pDirChunk,
		archive::Archive& a_OriginalArchive,
		std::vector<uint32_t> const& a_aOriginalDLFLData,
		std::string_view a_sDirTag, IdToChunk& a_aOut)
	{
		uint16_t numEntries = 0;
		std::vector<uint8_t> ids;
		std::vector<uint32_t> offs;
		std::vector<uint32_t> sizes;
		ReadDIRChunk(a_pDirChunk, a_sDirTag, numEntries, ids, offs, sizes);

		a_aOut = std::vector<parsing::Chunk*>(numEntries, nullptr);
		for (size_t id = 0; id < numEntries; id++)
		{
			if (ids[id] == 0)
			{
				continue;
			}

			const uint32_t originalAbs = a_aOriginalDLFLData[ids[id]] + offs[id];
			parsing::Chunk* ch = a_OriginalArchive.GetRoot().FindChunkAt(originalAbs);
			a_aOut[id] = ch;
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool HE0Builder::ParseDLFL(parsing::Chunk* a_pChunk, std::vector<uint32_t>& a_aOut)
	{
		// DLFL (RMIM byte offsets)
		assert(a_pChunk);
		if (!a_pChunk)
		{
			core::Log(core::LogLevel::Error, "Failed reading DLFL: Chunk was null.");
			return false;
		}

		assert(a_pChunk->GetTag() == parsing::DLFL_CHUNK_ID);
		if (a_pChunk->GetTag() != parsing::DLFL_CHUNK_ID)
		{
			core::Log(core::LogLevel::Error, "Failed reading DLFL: Chunk was not a DLFL chunk.");
			return false;
		}

		uint16_t numEntries = 0;
		core::DataStream data = a_pChunk->GetData();
		data.ReadLE16(numEntries);
		for (size_t i = 0; i < static_cast<size_t>(numEntries); i++)
		{
			uint32_t offset;
			data.ReadLE32(offset);
			a_aOut.push_back(offset);
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool HE0Builder::Build()
	{
		assert(m_pHE0);
		if (!m_pHE0)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: HE0 archive was null.");
			return false;
		}

		assert(m_pA);
		if (!m_pA)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: (A) archive was null.");
			return false;
		}

		assert(m_pDLFLChunk);
		if (!m_pDLFLChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DLFL chunk was null.");
			return false;
		}

		assert(m_pDLFLChunk->GetTag() == parsing::DLFL_CHUNK_ID);
		if (m_pDLFLChunk->GetTag() != parsing::DLFL_CHUNK_ID)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DLFL chunk did not have the DLFL tag.");
			return false;
		}

		if (!RebuildDLFL(m_pDLFLChunk, *m_pA))
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: Could not build DLFL.");
			return false;
		}

		std::vector<uint32_t> newDLFLData;
		if (!ParseDLFL(m_pDLFLChunk, newDLFLData))
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: Could not parse DLFL data.");
			return false;
		}

		assert(m_pDIRRChunk);
		if (!m_pDIRRChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DIRR chunk was null.");
			return false;
		}
		if (!RebuildDIRChunk(m_pDIRRChunk, m_aDIRR, newDLFLData, parsing::DIRR_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed building TALKie: Could not build DIRR chunk.");
			return false;
		}

		assert(m_pDIRSChunk);
		if (!m_pDIRSChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DIRS chunk was null.");
			return false;
		}
		if (!RebuildDIRChunk(m_pDIRSChunk, m_aDIRS, newDLFLData, parsing::DIRS_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed building TALKie: Could not build DIRS chunk.");
			return false;
		}

		assert(m_pDIRNChunk);
		if (!m_pDIRNChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DIRN chunk was null.");
			return false;
		}
		if (!RebuildDIRChunk(m_pDIRNChunk, m_aDIRN, newDLFLData, parsing::DIRN_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed building TALKie: Could not build DIRN chunk.");
			return false;
		}

		assert(m_pDIRCChunk);
		if (!m_pDIRCChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DIRC chunk was null.");
			return false;
		}
		if (!RebuildDIRChunk(m_pDIRCChunk, m_aDIRC, newDLFLData, parsing::DIRC_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed building TALKie: Could not build DIRC chunk.");
			return false;
		}

		assert(m_pDIRFChunk);
		if (!m_pDIRFChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DIRF chunk was null.");
			return false;
		}
		if (!RebuildDIRChunk(m_pDIRFChunk, m_aDIRF, newDLFLData, parsing::DIRF_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed building TALKie: Could not build DIRF chunk.");
			return false;
		}

		assert(m_pDIRMChunk);
		if (!m_pDIRMChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DIRM chunk was null.");
			return false;
		}
		if (!RebuildDIRChunk(m_pDIRMChunk, m_aDIRM, newDLFLData, parsing::DIRM_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed building TALKie: Could not build DIRM chunk.");
			return false;
		}

		assert(m_pDIRTChunk);
		if (!m_pDIRTChunk)
		{
			core::Log(core::LogLevel::Error, "Could not build HE0: DIRT chunk was null.");
			return false;
		}
		if (!RebuildDIRChunk(m_pDIRTChunk, m_aDIRT, newDLFLData, parsing::DIRT_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed building TALKie: Could not build DIRT chunk.");
			return false;
		}

		return true;
	}

	// Structure:
	// numRooms = uint16_t, number of RMIMs in the (a) + 1, LE.
	// rmimOffsets = uint32_t * numRooms, LE.
	// NOTE: 0 is a dummy note, numEntries is ALL LFLF chunks.
	// It saves LFLF chunk offsets. In practice, however, it always points to a RMIM because it does +8, no idea why.
	//---------------------------------------------------------------------
	bool HE0Builder::RebuildDLFL(parsing::Chunk* a_pChunk, archive::Archive& a_Archive)
	{
		// DLFL (RMIM byte offsets)
		assert(a_pChunk);
		if (!a_pChunk)
		{
			core::Log(core::LogLevel::Error, "Failed building DLFL: Chunk was null.");
			return false;
		}

		assert(a_pChunk->GetTag() == parsing::DLFL_CHUNK_ID);
		if (a_pChunk->GetTag() != parsing::DLFL_CHUNK_ID)
		{
			core::Log(core::LogLevel::Error, "Failed building DLFL: Chunk was not a DLFL chunk.");
			return false;
		}

		assert(a_Archive.GetType() == archive::ArchiveType::A);
		if (a_Archive.GetType() != archive::ArchiveType::A)
		{
			core::Log(core::LogLevel::Error, "Failed building DLFL: Provided archive was not an (A).");
			return false;
		}

		std::vector<parsing::Chunk*> lflfOffsets;

		a_Archive.GetRoot().TryFindChildren(parsing::LFLF_CHUNK_ID, lflfOffsets);
		lflfOffsets.insert(lflfOffsets.begin(), &a_Archive.GetRoot());

		uint32_t dlflSize = static_cast<uint32_t>(sizeof(uint16_t)) + (static_cast<uint32_t>(sizeof(uint32_t)) * static_cast<uint32_t>(lflfOffsets.size()));
		core::DataStream data(dlflSize);

		uint16_t numRooms = static_cast<uint16_t>(lflfOffsets.size());
		data.WriteLE16(numRooms);

		for (size_t i = 0; i < lflfOffsets.size(); i++)
		{
			uint32_t offsetFromRoot = static_cast<uint32_t>(lflfOffsets[i]->GetOffsetFromRoot());
			if (offsetFromRoot != 0)
			{
				offsetFromRoot += parsing::CHUNK_HEADER_SIZE;
			}
			data.WriteLE32(offsetFromRoot);
		}

		// Not necessary, but let's do it either way.
		a_pChunk->SetTag(parsing::DLFL_CHUNK_ID.data());
		a_pChunk->SetData(data);

		return true;
	}

	//---------------------------------------------------------------------
	bool HE0Builder::RebuildDIRChunk(
		parsing::Chunk* a_pChunk,
		std::vector<parsing::Chunk*> const& a_aIdToChunk,
		std::vector<uint32_t> const& a_aNewDLFLData,
		std::string_view a_sChunkID
	)
	{
		std::string chunkId = std::string(a_sChunkID);

		assert(a_pChunk);
		if (!a_pChunk)
		{
			core::Log(core::LogLevel::Error, "Failed building " + chunkId + ": Chunk was null.");
			return false;
		}

		assert(a_pChunk->GetTag() == a_sChunkID);
		if (a_pChunk->GetTag() != a_sChunkID)
		{
			core::Log(core::LogLevel::Error, "Failed building " + chunkId + ": Chunk was not a " + chunkId + " chunk.");
			return false;
		}

		// We need to read the old setup first because we need to check how many dummies were in the entry list.
		uint16_t originalNumEntries = 0;
		std::vector<uint8_t> originalIds;
		std::vector<uint32_t> originalOffsets;
		std::vector<uint32_t> originalSizes;
		if (!ReadDIRChunk(a_pChunk, a_sChunkID, originalNumEntries, originalIds, originalOffsets, originalSizes))
		{
			core::Log(core::LogLevel::Error, "Failed building " + chunkId + ": Could not read prior chunk version.");
			return false;
		}

		std::vector<uint8_t> newIds = originalIds;
		std::vector<uint32_t> newOffsets = originalOffsets;
		std::vector<uint32_t> newSizes = originalSizes;

		for (size_t id = 0; id < originalNumEntries; id++)
		{
			if (originalIds[id] == 0)
			{
				continue;
			}

			parsing::Chunk* ch = a_aIdToChunk[id];

			uint32_t newAbs = ch->GetOffsetFromRoot();
			uint8_t newLflf = originalIds[id];

			newOffsets[id] = newAbs - a_aNewDLFLData[newLflf];
			newSizes[id] = ch->WholeChunkSize();
			newIds[id] = originalIds[id];
		}

		uint32_t finalDirSize = 2 + newIds.size() * 1 + newOffsets.size() * 4 + newSizes.size() * 4;
		core::DataStream data = core::DataStream(finalDirSize);
		uint16_t numEntries = static_cast<uint16_t>(newIds.size());
		data.WriteLE16(numEntries);

		for (size_t i = 0; i < newIds.size(); i++)
		{
			data.Write(&newIds[i], sizeof(uint8_t));
		}

		for (size_t i = 0; i < newOffsets.size(); i++)
		{
			data.WriteLE32(newOffsets[i]);
		}

		for (size_t i = 0; i < newSizes.size(); i++)
		{
			data.WriteLE32(newSizes[i]);
		}

		// Not necessary, but let's do it either way.
		a_pChunk->SetTag(a_sChunkID.data());
		a_pChunk->SetData(data);

		return true;
	}
}