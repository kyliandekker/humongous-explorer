#include "./HE4Builder.h"

// external
#include <cassert>

#include <helib/archive/Archive.h>
#include <helib/archive/ArchiveType.h>
#include <helib/core/Log.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/parsing/chunks/sound/SGEN_Chunk.h>

namespace humongousexplorer::building
{
	//---------------------------------------------------------------------
	// HE4Builder
	//---------------------------------------------------------------------
	bool HE4Builder::Precache(archive::Archive& a_Archive)
	{
		assert(a_Archive.GetType() == archive::ArchiveType::HE4);
		if (a_Archive.GetType() != archive::ArchiveType::HE4)
		{
			core::Log(core::LogLevel::Error, "Could not precache HE4: Provided archive was not a HE4.");
			return false;
		}

		parsing::Chunk* songChunk = a_Archive.GetRoot().TryFindChild(parsing::SONG_CHUNK_ID);
		assert(songChunk);
		if (!songChunk)
		{
			core::Log(core::LogLevel::Error, "Could not precache HE4: Could not find SONG chunk.");
			return false;
		}

		std::vector<parsing::Chunk*> sgenChunks;
		if (!songChunk->TryFindChildren(parsing::SGEN_CHUNK_ID, sgenChunks))
		{
			core::Log(core::LogLevel::Error, "Could not precache HE4: Could not find SGEN chunks.");
			return false;
		}

		assert(!sgenChunks.empty());
		if (sgenChunks.empty())
		{
			core::Log(core::LogLevel::Error, "Could not precache HE4: No SGEN chunks were found.");
			return false;
		}

		for (parsing::Chunk* chunk : sgenChunks)
		{
			assert(chunk);
			if (!chunk)
			{
				core::Log(core::LogLevel::Error, "Could not precache HE4: SGEN chunk was null.");
				return false;
			}

			SGENEntry sgenEntry;
			sgenEntry.m_pSGENChunk = chunk;

			assert(!chunk->GetData().empty());
			if (chunk->GetData().empty())
			{
				core::Log(core::LogLevel::Error, "Could not precache HE4: SGEN data was empty.");
				return false;
			}

			const parsing::SGEN_Chunk* sgenData = chunk->GetData().dataAs<parsing::SGEN_Chunk>();
			assert(sgenData);
			if (!sgenData)
			{
				core::Log(core::LogLevel::Error, "Could not precache HE4: Something went wrong with loading SGEN data.");
				return false;
			}

			parsing::Chunk* digiChunk = songChunk->FindChunkAt(sgenData->songPos);
			assert(digiChunk);
			if (!digiChunk)
			{
				core::Log(core::LogLevel::Error, "Could not precache HE4: Could not find DIGI chunk.");
				return false;
			}

			assert(digiChunk->GetTag() == parsing::DIGI_CHUNK_ID);
			if (digiChunk->GetTag() != parsing::DIGI_CHUNK_ID)
			{
				core::Log(core::LogLevel::Error, "Could not precache HE4: Chunk at position " + std::to_string(sgenData->songPos) + " was not a DIGI chunk.");
				return false;
			}
			
			assert(digiChunk->WholeChunkSize() == sgenData->songSize);
			if (digiChunk->WholeChunkSize() != sgenData->songSize)
			{
				core::Log(core::LogLevel::Error, "Could not precache HE4: DIGI chunk size is not matching SGEN entry. Was this a failed rebuild?");
				return false;
			}

			sgenEntry.m_pDIGIChunk = digiChunk;

			m_aSGENs.push_back(sgenEntry);
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool HE4Builder::Build(archive::Archive& a_Archive)
	{
		for (const SGENEntry& sgenEntry : m_aSGENs)
		{
			parsing::Chunk* sgenChunk = sgenEntry.m_pSGENChunk;
			assert(sgenChunk);
			if (!sgenChunk)
			{
				core::Log(core::LogLevel::Error, "Could not rebuild HE4: SGEN chunk was null.");
				return false;
			}

			assert(!sgenChunk->GetData().empty());
			if (sgenChunk->GetData().empty())
			{
				core::Log(core::LogLevel::Error, "Could not rebuild HE4: SGEN data was empty.");
				return false;
			}

			parsing::SGEN_Chunk* sgenData = sgenChunk->GetData().dataAs<parsing::SGEN_Chunk>();
			assert(sgenData);
			if (!sgenData)
			{
				core::Log(core::LogLevel::Error, "Could not rebuild HE4: Something went wrong with loading SGEN data.");
				return false;
			}

			parsing::Chunk* digiChunk = sgenEntry.m_pDIGIChunk;
			assert(digiChunk);
			if (!digiChunk)
			{
				core::Log(core::LogLevel::Error, "Could not rebuild HE4: Could not find DIGI chunk.");
				return false;
			}

			assert(digiChunk->GetTag() == parsing::DIGI_CHUNK_ID);
			if (digiChunk->GetTag() != parsing::DIGI_CHUNK_ID)
			{
				core::Log(core::LogLevel::Error, "Could not rebuild HE4: Chunk at position " + std::to_string(sgenData->songPos) + " was not a DIGI chunk.");
				return false;
			}

			sgenData->songPos = digiChunk->GetOffsetFromRoot();
			sgenData->songSize = digiChunk->WholeChunkSize();
		}

		return true;
	}
}