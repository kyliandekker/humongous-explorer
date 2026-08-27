#include "./TalkResource.h"

#include <cassert>
#include <unordered_map>

#include <helib/core/DataStream.h>
#include <helib/archive/Archive.h>
#include <set>
#include <helib/file/file.h>
#include <helib/core/Log.h>
#include <helib/parsing/Chunk.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/parsing/chunks/sound/HSHD_Chunk.h>

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	// TalkResource
	//---------------------------------------------------------------------
	void TalkResource::Initialize(parsing::Chunk* a_pChunk)
	{
		assert(a_pChunk->GetTag() == parsing::TALK_CHUNK_ID);
		if (a_pChunk->GetTag() != parsing::TALK_CHUNK_ID)
		{
			core::Log(core::LogLevel::Error, "Failed loading TALKie: Chunk was not a TALK chunk.");
			return;
		}

		m_aRelevantChunks.push_back(a_pChunk);

		parsing::Chunk* hshdChunk = a_pChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
		assert(hshdChunk);
		if (!hshdChunk)
		{
			core::Log(core::LogLevel::Error, "Failed loading TALKie: Could not find HSHD chunk.");
			return;
		}

		m_aRelevantChunks.push_back(hshdChunk);

		parsing::Chunk* sdatChunk = a_pChunk->TryFindChild(parsing::SDAT_CHUNK_ID);
		assert(sdatChunk);
		if (!sdatChunk)
		{
			core::Log(core::LogLevel::Error, "Failed loading TALKie: Could not find SDAT chunk.");
			return;
		}

		m_aRelevantChunks.push_back(sdatChunk);

		parsing::Chunk* sbngChunk = a_pChunk->TryFindChild(parsing::SBNG_CHUNK_ID);
		if (sbngChunk)
		{
			m_aRelevantChunks.push_back(sbngChunk);
		}
	}

	//---------------------------------------------------------------------
	// Structure:
	// numRooms = uint16_t, number of RMIMs in the (a) + 1, LE.
	// rmimOffsets = uint32_t * numRooms, LE.
	// NOTE: 0 is a dummy note, numEntries is ALL LFLF chunks.
	// It saves LFLF chunk offsets. In practice, however, it always points to a RMIM because it does +8, no idea why.
	bool RebuildDLFL(parsing::Chunk* a_pChunk, archive::Archive& a_Archive)
	{
		// DLFL (RMIM byte offsets)
		assert(a_pChunk);
		if (!a_pChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding DLFL: Chunk was null.");
			return false;
		}

		assert(a_pChunk->GetTag() == parsing::DLFL_CHUNK_ID);
		if (a_pChunk->GetTag() != parsing::DLFL_CHUNK_ID)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding DLFL: Chunk was not a DLFL chunk.");
			return false;
		}

		assert(a_Archive.GetType() == archive::ArchiveType::A);
		if (a_Archive.GetType() != archive::ArchiveType::A)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding DLFL: Provided archive was not an (A).");
			return false;
		}

		std::vector<parsing::Chunk*> lflfOffsets;

		a_Archive.GetRoot().TryFindChildren(parsing::LFLF_CHUNK_ID, lflfOffsets);
		lflfOffsets.insert(lflfOffsets.begin(), &a_Archive.GetRoot());

		uint32_t dlflSize = static_cast<uint32_t>(sizeof(uint16_t)) + (static_cast<uint32_t>(sizeof(uint32_t)) * static_cast<uint32_t>(lflfOffsets.size()));
		core::DataStream data(dlflSize);

		uint16_t numRooms = static_cast<uint16_t>(lflfOffsets.size());
		data.Write(&numRooms, sizeof(numRooms));

		for (size_t i = 0; i < lflfOffsets.size(); i++)
		{
			uint32_t offsetFromRoot = static_cast<uint32_t>(lflfOffsets[i]->GetOffsetFromRoot());
			if (offsetFromRoot != 0)
			{
				offsetFromRoot += parsing::CHUNK_HEADER_SIZE;
			}
			data.Write(&offsetFromRoot, sizeof(offsetFromRoot));
		}

		// Not necessary, but let's do it either way.
		a_pChunk->SetTag(parsing::DLFL_CHUNK_ID.data());
		a_pChunk->SetData(data);

		return true;
	}

	//---------------------------------------------------------------------
	// Structure:
	// numRooms = uint16_t, number of RMIMs in the (a) + 1, LE.
	// rmimOffsets = uint32_t * numRooms, LE.
	// NOTE: 0 is a dummy note, numEntries is ALL LFLF chunks.
	// It saves LFLF chunk offsets. In practice, however, it always points to a RMIM because it does +8, no idea why.
	bool ReadDLFL(parsing::Chunk* a_pChunk, uint16_t& a_iNumEntries, std::vector<uint32_t>& a_aOffsets)
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

		core::DataStream data = a_pChunk->GetData();
		data.Read(&a_iNumEntries, sizeof(a_iNumEntries), 1);
		for (size_t i = 0; i < static_cast<size_t>(a_iNumEntries); i++)
		{
			uint32_t offset;
			data.Read(&offset, sizeof(offset), 1);
			a_aOffsets.push_back(offset);
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool ReadDIRChunk(parsing::Chunk* a_pChunk, const std::string_view& a_sChunkID, uint16_t& a_iNumEntries, std::vector<uint8_t>& a_aIds, std::vector<uint32_t>& a_aOffsets, std::vector<uint32_t>& a_aSizes)
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
		data.Read(&a_iNumEntries, sizeof(a_iNumEntries), 1);
		for (size_t i = 0; i < static_cast<size_t>(a_iNumEntries); i++)
		{
			uint8_t id;
			data.Read(&id, sizeof(id), 1);
			a_aIds.push_back(id);
		}

		for (size_t i = 0; i < static_cast<size_t>(a_iNumEntries); i++)
		{
			uint32_t offset;
			data.Read(&offset, sizeof(offset), 1);
			a_aOffsets.push_back(offset);
		}

		size_t tell = data.Tell();
		size_t dataSize = a_pChunk->ChunkSize();

		// Some HE games have size also appended in the entries.
		if (tell < dataSize)
		{
			int32_t diff = dataSize - tell;
			assert(diff == (static_cast<uint32_t>(sizeof(uint32_t)) * a_iNumEntries)); // Double check it is exactly a size array.

			for (size_t i = 0; i < static_cast<size_t>(a_iNumEntries); i++)
			{
				uint32_t offset;
				data.Read(&offset, sizeof(offset), 1);
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
	std::vector<parsing::Chunk*> BuildIdToChunk(
		parsing::Chunk* a_pDirChunk,
		archive::Archive& a_OldArchive,
		std::vector<uint32_t> const& a_aOldDLFLData,
		std::string_view a_sDirTag)
	{
		uint16_t numEntries = 0;
		std::vector<uint8_t> ids;
		std::vector<uint32_t> offs;
		std::vector<uint32_t> sizes;
		ReadDIRChunk(a_pDirChunk, a_sDirTag, numEntries, ids, offs, sizes);

		std::vector<parsing::Chunk*> idToChunk = std::vector<parsing::Chunk*>(numEntries, nullptr);
		for (size_t id = 0; id < numEntries; id++)
		{
			if (ids[id] == 0)
			{
				continue;
			}

			uint32_t oldAbs = a_aOldDLFLData[ids[id]] + offs[id];
			parsing::Chunk* ch = a_OldArchive.GetRoot().FindChunkAt(oldAbs);
			idToChunk[id] = ch;
		}

		return idToChunk;
	}

	//---------------------------------------------------------------------
	// DIR chunks are index chunks in HE0.
	// numChunks = uint16_t, number of chunks in the (a) + 1, LE.
	// roomIds = uint8_t * numChunks, LE.
	// offsets = uint32_t * numChunks, LE.
	//
	// DIRR = RMDA byte offsets + 16.
	// DIRS = SCRP byte offsets + 16 (LSCR, LSC2, EXCD, ENCD are inside the RMDA).
	// DIRN = SOUN/DIGI/TALK byte offsets + 16 (only TALK inside the (A) numEntries).
	// DIRC = COST/AKOS byte offsets + 16 (Costume/Actor).
	// DIRF = CHAR byte offsets + 16 (Characters).
	// DIRM = (MULT/AWIZ) byte offsets + 16 (Movie/Animation).
	// DIRT = TLKE byte offsets + 16 (Old TALKs).
	// 
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
	bool RebuildDIRChunk(
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
			core::Log(core::LogLevel::Error, "Failed rebuilding " + chunkId + ": Chunk was null.");
			return false;
		}

		assert(a_pChunk->GetTag() == a_sChunkID);
		if (a_pChunk->GetTag() != a_sChunkID)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding " + chunkId + ": Chunk was not a " + chunkId + " chunk.");
			return false;
		}

		// We need to read the old setup first because we need to check how many dummies were in the entry list.
		uint16_t originalNumEntries = 0;
		std::vector<uint8_t> originalIds;
		std::vector<uint32_t> originalOffsets;
		std::vector<uint32_t> originalSizes;
		if (!ReadDIRChunk(a_pChunk, a_sChunkID, originalNumEntries, originalIds, originalOffsets, originalSizes))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding " + chunkId + ": Could not read prior chunk version.");
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
		data.Write(&numEntries, sizeof(numEntries));

		for (size_t i = 0; i < newIds.size(); i++)
		{
			data.Write(&newIds[i], sizeof(uint8_t));
		}

		for (size_t i = 0; i < newOffsets.size(); i++)
		{
			data.Write(&newOffsets[i], sizeof(uint32_t));
		}

		for (size_t i = 0; i < newSizes.size(); i++)
		{
			data.Write(&newSizes[i], sizeof(uint32_t));
		}

		// Not necessary, but let's do it either way.
		a_pChunk->SetTag(a_sChunkID.data());
		a_pChunk->SetData(data);

		return true;
	}

	//---------------------------------------------------------------------
	bool TalkResource::Replace(const core::Data& a_Data)
	{
		// ==============================================================
		// 1. Validate that all necessary chunks are here.
		// ==============================================================

		parsing::Chunk* talkChunk = nullptr;
		parsing::Chunk* hshdChunk = nullptr;
		parsing::Chunk* sdatChunk = nullptr;
		parsing::Chunk* sbngChunk = nullptr;
		for (parsing::Chunk* chunk : m_aRelevantChunks)
		{
			if (chunk->GetTag() == parsing::TALK_CHUNK_ID)
			{
				talkChunk = chunk;
			}
			else if (chunk->GetTag() == parsing::HSHD_CHUNK_ID)
			{
				hshdChunk = chunk;
			}
			else if (chunk->GetTag() == parsing::SDAT_CHUNK_ID)
			{
				sdatChunk = chunk;
			}
			else if (chunk->GetTag() == parsing::SBNG_CHUNK_ID)
			{
				sbngChunk = chunk;
			}
		}

		assert(talkChunk);
		if (!talkChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: TALK chunk was null.");
			return false;
		}
		assert(hshdChunk);
		if (!hshdChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: HSHD chunk was null.");
			return false;
		}
		assert(sdatChunk);
		if (!sdatChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: SDAT chunk was null.");
			return false;
		}

		// We do not necessarily NEED the SBNG.

		parsing::Chunk* tlkbChunk = talkChunk->GetRoot();
		assert(tlkbChunk);
		if (!tlkbChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Root chunk was null.");
			return false;
		}
		assert(tlkbChunk->GetTag() == parsing::TLKB_CHUNK_ID);
		if (tlkbChunk->GetTag() != parsing::TLKB_CHUNK_ID)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Root was not a TLKB chunk.");
			return false;
		}

		// ==============================================================
		// 2. Get the (A) and HE0.
		// ==============================================================

		archive::Archive* he0 = m_pHE0;
		archive::Archive* a = m_pA;

		// TODO: We need global storage for archives. Until then, this code is commmented out.
		//for (std::unique_ptr<archive::Archive>& archive : set.GetArchives())
		//{
		//	if (archive->GetType() == archive::ArchiveType::HE0)
		//	{
		//		he0 = archive.get();
		//	}
		//}

		if (!he0)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find HE0 archive.");
			return false;
		}

		if (!a)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find (A) archive.");
			return false;
		}

		// ==============================================================
		// 3. Find the DLFL and DIR* chunks.
		// We basically want to do several things here:
		//		1. Read the DLFL so that we know the offsets of LFLFs based on the ID that has been given to them.
		//		2. Binds chunk pointers to the IDs given in the DIR* chunks. We do this for these reasons:
		//			- Rebuilding the (A) makes offsets shift and sizes increase. Looking at old offsets and calculating
		//			  differences won't work. Adding a difference will not guarantee it is the same chunk as before.
		// ==============================================================

		// DLFL (RMIM byte offsets)
		parsing::Chunk* dlflChunk = he0->GetRoot().TryFindChild(parsing::DLFL_CHUNK_ID);
		assert(dlflChunk);
		if (!dlflChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DLFL chunk in HE0 archive.");
			return false;
		}

		uint16_t originalNumDLFLEntries = 0;
		std::vector<uint32_t> originalDLFLOffsets;
		if (!ReadDLFL(dlflChunk, originalNumDLFLEntries, originalDLFLOffsets))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not read DLFL chunk in HE0 archive.");
			return false;
		}

		parsing::Chunk* dirrChunk = he0->GetRoot().TryFindChild(parsing::DIRR_CHUNK_ID);
		assert(dirrChunk);
		if (!dirrChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DIRR chunk.");
			return false;
		}
		std::vector<parsing::Chunk*> idToChunkR = BuildIdToChunk(dirrChunk, *a, originalDLFLOffsets, parsing::DIRR_CHUNK_ID);

		parsing::Chunk* dirsChunk = he0->GetRoot().TryFindChild(parsing::DIRS_CHUNK_ID);
		assert(dirsChunk);
		if (!dirsChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DIRS chunk.");
			return false;
		}
		std::vector<parsing::Chunk*> idToChunkS = BuildIdToChunk(dirsChunk, *a, originalDLFLOffsets, parsing::DIRS_CHUNK_ID);

		parsing::Chunk* dirnChunk = he0->GetRoot().TryFindChild(parsing::DIRN_CHUNK_ID);
		assert(dirnChunk);
		if (!dirnChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DIRN chunk.");
			return false;
		}
		std::vector<parsing::Chunk*> idToChunkN = BuildIdToChunk(dirnChunk, *a, originalDLFLOffsets, parsing::DIRN_CHUNK_ID);

		parsing::Chunk* dircChunk = he0->GetRoot().TryFindChild(parsing::DIRC_CHUNK_ID);
		assert(dircChunk);
		if (!dircChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DIRC chunk.");
			return false;
		}
		std::vector<parsing::Chunk*> idToChunkC = BuildIdToChunk(dircChunk, *a, originalDLFLOffsets, parsing::DIRC_CHUNK_ID);

		parsing::Chunk* dirfChunk = he0->GetRoot().TryFindChild(parsing::DIRF_CHUNK_ID);
		assert(dirfChunk);
		if (!dirfChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DIRF chunk.");
			return false;
		}
		std::vector<parsing::Chunk*> idToChunkF = BuildIdToChunk(dirfChunk, *a, originalDLFLOffsets, parsing::DIRF_CHUNK_ID);

		parsing::Chunk* dirmChunk = he0->GetRoot().TryFindChild(parsing::DIRM_CHUNK_ID);
		assert(dirmChunk);
		if (!dirmChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DIRM chunk.");
			return false;
		}
		std::vector<parsing::Chunk*> idToChunkM = BuildIdToChunk(dirmChunk, *a, originalDLFLOffsets, parsing::DIRM_CHUNK_ID);

		// Even tho DIRT will most likely be empty, the chunk itself should still be present.
		parsing::Chunk* dirtChunk = he0->GetRoot().TryFindChild(parsing::DIRT_CHUNK_ID);
		assert(dirtChunk);
		if (!dirtChunk)
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not find DIRT chunk.");
			return false;
		}
		std::vector<parsing::Chunk*> idToChunkT = BuildIdToChunk(dirtChunk, *a, originalDLFLOffsets, parsing::DIRT_CHUNK_ID);

		// ==============================================================
		// 4. Rebuild the TALKie and the (A) scripts that reference it.
		// ==============================================================

		// Get the new talk size.
		size_t dataSize = a_Data.size();
		int64_t diff = static_cast<int64_t>(a_Data.size()) - static_cast<int64_t>(sdatChunk->GetData().size());

		size_t replacedTalkOffset = talkChunk->GetOffsetFromRoot();
		for (std::unique_ptr<parsing::Chunk>& chunkPtr : tlkbChunk->GetChildren())
		{
			if (chunkPtr->GetOffsetFromRoot() < replacedTalkOffset)
			{
				continue;
			}
			parsing::Chunk* chunk = chunkPtr.get();
			bool isReplacedChunk = chunk == talkChunk;

			size_t originalTalkOffset = chunk->GetOffsetFromRoot();
			size_t newTalkOffset = originalTalkOffset;

			// We only need to calculate a new offset for chunks that appear after the replaced chunk.
			if (!isReplacedChunk)
			{
				newTalkOffset = originalTalkOffset + diff;
			}

			size_t originalTalkSize = chunk->WholeChunkSize();
			size_t newTalkSize = originalTalkSize;

			// We only need to calculate a new size for the replaced chunk.
			// Chunks that appear after the replaced chunk will not have their size changed.
			if (isReplacedChunk)
			{
				newTalkSize = originalTalkSize + diff;
			}

			int32_t talkOffsetStrDiff = static_cast<int32_t>(std::to_string(newTalkOffset).size()) - static_cast<int32_t>(std::to_string(originalTalkOffset).size());
			int32_t talkSizeStrDiff = static_cast<int32_t>(std::to_string(newTalkSize).size()) - static_cast<int32_t>(std::to_string(originalTalkSize).size());

			// Update scripts containing originalTalkOffset and talkSize and replace them with newTalkOffset and newTalkSize.
		}

		// Replace the SDAT chunk with the new data.
		sdatChunk->SetData(a_Data);

		// ==============================================================
		// 5. Rebuild the DLFL with new offsets.
		// ==============================================================

		if (!RebuildDLFL(dlflChunk, *a))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DLFL.");
			return false;
		}

		uint16_t newNumDLFLEntries = 0;
		std::vector<uint32_t> newDlflOffsets;
		if (!ReadDLFL(dlflChunk, newNumDLFLEntries, newDlflOffsets))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not read DLFL chunk in HE0 archive.");
			return false;
		}

		// ==============================================================
		// 5. Rebuild the DIR* chunks by checking the idToChunk mappings we have made from earlier.
		// ==============================================================

		if (!RebuildDIRChunk(dirrChunk, idToChunkR, newDlflOffsets, parsing::DIRR_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DIRR chunk.");
			return false;
		}
		if (!RebuildDIRChunk(dirsChunk, idToChunkS, newDlflOffsets, parsing::DIRS_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DIRS chunk.");
			return false;
		}
		if (!RebuildDIRChunk(dirnChunk, idToChunkN, newDlflOffsets, parsing::DIRN_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DIRN chunk.");
			return false;
		}
		if (!RebuildDIRChunk(dircChunk, idToChunkC, newDlflOffsets, parsing::DIRC_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DIRC chunk.");
			return false;
		}
		if (!RebuildDIRChunk(dirfChunk, idToChunkF, newDlflOffsets, parsing::DIRF_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DIRF chunk.");
			return false;
		}
		if (!RebuildDIRChunk(dirmChunk, idToChunkM, newDlflOffsets, parsing::DIRM_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DIRM chunk.");
			return false;
		}
		if (!RebuildDIRChunk(dirtChunk, idToChunkT, newDlflOffsets, parsing::DIRT_CHUNK_ID))
		{
			core::Log(core::LogLevel::Error, "Failed rebuilding TALKie: Could not rebuild DIRT chunk.");
			return false;
		}

		core::DataStream dirsData(dirsChunk->WholeChunkSize());
		dirsChunk->ToData(dirsData);
		file::SaveFile("C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/DIRS_Test2", dirsData);

		return true;
	}
}