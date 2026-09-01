#include "./HE4Builder.h"

// external
#include <cassert>

#include <helib/archive/Archive.h>
#include <helib/archive/ArchiveSet.h>
#include <helib/archive/ArchiveType.h>
#include <helib/core/Log.h>
#include <helib/core/Memory.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/parsing/chunks/sound/SGEN_Chunk.h>

namespace humongousexplorer::building
{
	//---------------------------------------------------------------------
	// SGENEntry
	//---------------------------------------------------------------------
	parsing::Chunk* SGENEntry::GetSGENChunk()
	{
		return m_pSGENChunk;
	}

	//---------------------------------------------------------------------
	parsing::Chunk* SGENEntry::GetDIGIChunk()
	{
		return m_pDIGIChunk;
	}

	//---------------------------------------------------------------------
	void SGENEntry::SetSGENChunk(parsing::Chunk* a_pSGENChunk)
	{
		m_pSGENChunk = a_pSGENChunk;
	}

	//---------------------------------------------------------------------
	void SGENEntry::SetDIGIChunk(parsing::Chunk* a_pDIGIChunk)
	{
		m_pDIGIChunk = a_pDIGIChunk;
	}

	//---------------------------------------------------------------------
	// HE4Builder
	//---------------------------------------------------------------------
	bool HE4Builder::Bind(archive::ArchiveSet& a_ArchiveSet)
	{
		m_pHE4 = nullptr;
		m_aSGENs.clear();

		for (std::unique_ptr<archive::Archive>& archive : a_ArchiveSet.GetArchives())
		{
			if (archive->GetType() == archive::ArchiveType::HE4)
			{
				m_pHE4 = archive.get();
				break;
			}
		}

		assert(m_pHE4);
		if (!m_pHE4)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE4: Could not find HE4.");
			return false;
		}

		parsing::Chunk* songChunk = m_pHE4->GetRoot().TryFindChild(parsing::SONG_CHUNK_ID);
		assert(songChunk);
		if (!songChunk)
		{
			core::Log(core::LogLevel::Error, "Could not bind HE4: Could not find SONG chunk.");
			return false;
		}

		std::vector<parsing::Chunk*> sgenChunks;
		if (!songChunk->TryFindChildren(parsing::SGEN_CHUNK_ID, sgenChunks))
		{
			core::Log(core::LogLevel::Error, "Could not bind HE4: Could not find SGEN chunks.");
			return false;
		}

		assert(!sgenChunks.empty());
		if (sgenChunks.empty())
		{
			core::Log(core::LogLevel::Error, "Could not bind HE4: No SGEN chunks were found.");
			return false;
		}

		for (parsing::Chunk* chunk : sgenChunks)
		{
			assert(chunk);
			if (!chunk)
			{
				core::Log(core::LogLevel::Error, "Could not bind HE4: SGEN chunk was null.");
				return false;
			}

			SGENEntry sgenEntry;
			sgenEntry.SetSGENChunk(chunk);

			assert(!chunk->GetData().empty());
			if (chunk->GetData().empty())
			{
				core::Log(core::LogLevel::Error, "Could not bind HE4: SGEN data was empty.");
				return false;
			}

			const core::Data& sgenRaw = chunk->GetData();
			if (sgenRaw.size() < 13)
			{
				core::Log(core::LogLevel::Error, "Could not bind HE4: SGEN data too small.");
				return false;
			}
			const unsigned char* sgenBytes = static_cast<const unsigned char*>(sgenRaw.data());
			uint32_t songPos = core::ReadLE32(sgenBytes + 4);
			uint32_t songSize = core::ReadLE32(sgenBytes + 8);

			parsing::Chunk* digiChunk = songChunk->FindChunkAt(songPos);
			assert(digiChunk);
			if (!digiChunk)
			{
				core::Log(core::LogLevel::Error, "Could not bind HE4: Could not find DIGI chunk.");
				return false;
			}

			assert(digiChunk->GetTag() == parsing::DIGI_CHUNK_ID);
			if (digiChunk->GetTag() != parsing::DIGI_CHUNK_ID)
			{
				core::Log(core::LogLevel::Error, "Could not bind HE4: Chunk at position " + std::to_string(songPos) + " was not a DIGI chunk.");
				return false;
			}

			assert(digiChunk->WholeChunkSize() == songSize);
			if (digiChunk->WholeChunkSize() != songSize)
			{
				core::Log(core::LogLevel::Error, "Could not bind HE4: DIGI chunk size is not matching SGEN entry. Was this a failed build?");
				return false;
			}

			sgenEntry.SetDIGIChunk(digiChunk);

			m_aSGENs.push_back(sgenEntry);
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool HE4Builder::Build()
	{
		assert(m_pHE4);
		if (!m_pHE4)
		{
			core::Log(core::LogLevel::Error, "Could not build HE4: HE4 archive was null.");
			return false;
		}

		for (SGENEntry& sgenEntry : m_aSGENs)
		{
			parsing::Chunk* sgenChunk = sgenEntry.GetSGENChunk();
			assert(sgenChunk);
			if (!sgenChunk)
			{
				core::Log(core::LogLevel::Error, "Could not build HE4: SGEN chunk was null.");
				return false;
			}

			core::Data& sgenRaw = sgenChunk->GetData();
			if (sgenRaw.empty() || sgenRaw.size() < 13)
			{
				core::Log(core::LogLevel::Error, "Could not build HE4: SGEN data was empty or too small.");
				return false;
			}

			parsing::Chunk* digiChunk = sgenEntry.GetDIGIChunk();
			assert(digiChunk);
			if (!digiChunk)
			{
				core::Log(core::LogLevel::Error, "Could not build HE4: Could not find DIGI chunk.");
				return false;
			}

			const unsigned char* chkBytes = static_cast<const unsigned char*>(sgenRaw.data());
			uint32_t curPos = core::ReadLE32(chkBytes + 4);
			assert(digiChunk->GetTag() == parsing::DIGI_CHUNK_ID);
			if (digiChunk->GetTag() != parsing::DIGI_CHUNK_ID)
			{
				core::Log(core::LogLevel::Error, "Could not build HE4: Chunk at position " + std::to_string(curPos) + " was not a DIGI chunk.");
				return false;
			}

			unsigned char* mutableBytes = static_cast<unsigned char*>(sgenRaw.data());
			core::WriteLE32(mutableBytes + 4, static_cast<uint32_t>(digiChunk->GetOffsetFromRoot()));
			core::WriteLE32(mutableBytes + 8, static_cast<uint32_t>(digiChunk->WholeChunkSize()));
		}

		return true;
	}
}