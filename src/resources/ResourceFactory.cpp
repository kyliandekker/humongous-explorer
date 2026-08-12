#include "./ResourceFactory.h"

#include "core/Memory.h"

#include "parsing/HEParser.h"

#include "humongous/ChunkIDs.h"

#include "resources/Resource.h"

#include "humongous/sound/HSHD_Chunk.h"
#include "humongous/sound/SGEN_Chunk.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	std::unique_ptr<Resource> ResourceFactory::GetResource(parsing::Chunk* a_pChunk, const std::string& a_sFallbackName)
	{
		if (core::chunkcmp(a_pChunk->m_sTag, parsing::TALK_CHUNK_ID) == 0)
		{
			parsing::Chunk* hshd = a_pChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
			if (!hshd)
			{
				return nullptr;
			}
			headers::HSHD_Chunk* hshdData = hshd->m_Data.dataAs<headers::HSHD_Chunk>();

			std::unique_ptr<TalkResource> talkResource = std::make_unique<TalkResource>();
			talkResource->SetName(a_sFallbackName);
			talkResource->SetDataChunk(a_pChunk->TryFindChild(parsing::SDAT_CHUNK_ID));
			talkResource->SetLipSyncChunk(a_pChunk->TryFindChild(parsing::SBNG_CHUNK_ID));
			talkResource->SetSampleRate(hshdData->sampleRate);

			return talkResource;
		}
		if (core::chunkcmp(a_pChunk->m_sTag, parsing::SGEN_CHUNK_ID) == 0)
		{
			headers::SGEN_Chunk* sgenData = a_pChunk->m_Data.dataAs<headers::SGEN_Chunk>();
			parsing::Chunk* digiChunk = a_pChunk->GetRoot()->FindChunkAt(sgenData->songPos);
			if (!digiChunk)
			{
				return nullptr;
			}

			parsing::Chunk* hshd = digiChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
			if (!hshd)
			{
				return nullptr;
			}
			headers::HSHD_Chunk* hshdData = hshd->m_Data.dataAs<headers::HSHD_Chunk>();

			std::unique_ptr<SongResource> songResource = std::make_unique<SongResource>();
			songResource->SetName(std::to_string(sgenData->id));
			songResource->SetDataChunk(digiChunk->TryFindChild(parsing::SDAT_CHUNK_ID));
			songResource->SetSampleRate(hshdData->sampleRate);

			return songResource;
		}
		return nullptr;
	}
}