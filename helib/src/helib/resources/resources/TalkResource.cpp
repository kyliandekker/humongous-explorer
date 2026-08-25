#include "./TalkResource.h"

#include <cassert>
#include <unordered_map>

#include <helib/archive/Archive.h>
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
			return;
		}

		m_aRelevantChunks.push_back(a_pChunk);

		parsing::Chunk* hshdChunk = a_pChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
		assert(hshdChunk);
		if (!hshdChunk)
		{
			return;
		}

		m_aRelevantChunks.push_back(hshdChunk);

		parsing::Chunk* sdatChunk = a_pChunk->TryFindChild(parsing::SDAT_CHUNK_ID);
		assert(sdatChunk);
		if (!sdatChunk)
		{
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
	bool TalkResource::Replace(const core::Data& a_Data)
	{
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
			return false;
		}
		assert(hshdChunk);
		if (!hshdChunk)
		{
			return false;
		}
		assert(sdatChunk);
		if (!sdatChunk)
		{
			return false;
		}

		parsing::Chunk* tlkbChunk = talkChunk->GetParent();
		assert(tlkbChunk);
		if (!tlkbChunk)
		{
			return false;
		}
		assert(tlkbChunk->GetTag() == parsing::TLKB_CHUNK_ID);
		if (tlkbChunk->GetTag() != parsing::TLKB_CHUNK_ID)
		{
			return false;
		}

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

		// Rebuild HE0:
		// This should be a static function.

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
			return 0;
		}

		if (!a)
		{
			return 0;
		}

		// DLFL (RMIM byte offsets)
		parsing::Chunk* dlflChunk = he0->GetRoot().TryFindChild(parsing::DLFL_CHUNK_ID);
		assert(dlflChunk);
		if (!dlflChunk)
		{
			return false;
		}

		// DIRR (RMDA byte offsets)
		parsing::Chunk* dirrChunk = he0->GetRoot().TryFindChild(parsing::DIRR_CHUNK_ID);
		assert(dirrChunk);
		if (!dirrChunk)
		{
			return false;
		}

		// DIRS (SCRP byte offsets)
		// Why only SCRP? SCRP is outside of the RMDA (LSCR, LSC2, EXCD, ENCD are inside the RMDA).
		parsing::Chunk* dirsChunk = he0->GetRoot().TryFindChild(parsing::DIRS_CHUNK_ID);
		assert(dirsChunk);
		if (!dirsChunk)
		{
			return false;
		}

		// DIRN (SOUN/DIGI/TALK byte offsets)
		// SOUN, DIGI and TALK outside of the RMDA.
		parsing::Chunk* dirnChunk = he0->GetRoot().TryFindChild(parsing::DIRN_CHUNK_ID);
		assert(dirnChunk);
		if (!dirnChunk)
		{
			return false;
		}

		// DIRC (COST/AKOS byte offsets) (Costume/Actor)
		parsing::Chunk* dircChunk = he0->GetRoot().TryFindChild(parsing::DIRC_CHUNK_ID);
		assert(dircChunk);
		if (!dircChunk)
		{
			return false;
		}

		// DIRF (Characters, CHAR)
		parsing::Chunk* dirfChunk = he0->GetRoot().TryFindChild(parsing::DIRF_CHUNK_ID);
		assert(dirfChunk);
		if (!dirfChunk)
		{
			return false;
		}

		// DIRM (Movie/Animation (MULT/AWIZ))
		parsing::Chunk* dirmChunk = he0->GetRoot().TryFindChild(parsing::DIRM_CHUNK_ID);
		assert(dirmChunk);
		if (!dirmChunk)
		{
			return false;
		}

		// DIRT (Talkie, TLKE (not applicable, we have TLKB).
		parsing::Chunk* dirtChunk = he0->GetRoot().TryFindChild(parsing::DIRT_CHUNK_ID);

		struct dlfl_offset
		{
			uint16_t count; // Starts with number of rooms.
			std::vector<uint32_t> lflfOffsets; // List of offsets, LE.
		};

		struct HE_DIR
		{
			uint16_t count; // DIR chunks start with a count.
			std::vector<uint8_t> lflfId; // After that it has a list of LFLF IDs. These can appear multiple times.
			std::vector<uint32_t> offset; // After that whole list, the offsets come. It mimicks a map/dictionary, but very badly. LE.
		};

		std::unordered_map<parsing::Chunk*, std::vector<std::string_view>> chunks_in_index_pair =
		{
			{ dirrChunk, { parsing::RMDA_CHUNK_ID } },
			{ dirsChunk, { parsing::SCRP_CHUNK_ID } },
			{ dirnChunk, { parsing::SOUN_CHUNK_ID, parsing::TALK_CHUNK_ID, parsing::DIGI_CHUNK_ID } },
			{ dircChunk, { parsing::AKOS_CHUNK_ID } },
			{ dirfChunk, { parsing::CHAR_CHUNK_ID } },
			{ dirmChunk, { parsing::MULT_CHUNK_ID, parsing::AWIZ_CHUNK_ID } },
			{ dirtChunk, { parsing::TLKE_CHUNK_ID } },
		};

		return true;
	}
}