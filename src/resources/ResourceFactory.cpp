#include "./ResourceFactory.h"

#include "core/Memory.h"

#include "parsing/ChunkParser.h"
#include "parsing/Chunk.h"

#include "parsing/ChunkIDs.h"

#include "resources/Resource.h"

#include "parsing/chunks/sound/HSHD_Chunk.h"
#include "parsing/chunks/sound/SGEN_Chunk.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	std::unique_ptr<Resource> ResourceFactory::GetResource(parsing::Chunk* a_pChunk, const std::string& a_sFallbackName)
	{
		if (core::chunkcmp(a_pChunk->GetTag(), parsing::LFLF_CHUNK_ID) == 0)
		{
			std::unique_ptr<RoomResource> roomResource = std::make_unique<RoomResource>();
			roomResource->SetName(a_sFallbackName);
			return roomResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::TALK_CHUNK_ID) == 0)
		{
			const parsing::Chunk* hshd = a_pChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
			if (!hshd)
			{
				return nullptr;
			}
			const parsing::HSHD_Chunk* hshdData = hshd->GetData().dataAs<parsing::HSHD_Chunk>();

			std::unique_ptr<TalkResource> talkResource = std::make_unique<TalkResource>();
			talkResource->SetName(a_sFallbackName);
			talkResource->SetDataChunk(a_pChunk->TryFindChild(parsing::SDAT_CHUNK_ID));
			talkResource->SetLipSyncChunk(a_pChunk->TryFindChild(parsing::SBNG_CHUNK_ID));
			talkResource->SetSampleRate(hshdData->sampleRate);

			return talkResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::SGEN_CHUNK_ID) == 0)
		{
			parsing::SGEN_Chunk* sgenData = a_pChunk->GetData().dataAs<parsing::SGEN_Chunk>();
			parsing::Chunk* root = a_pChunk->GetRoot();
			parsing::Chunk* digiChunk = root->FindChunkAt(sgenData->songPos);
			if (!digiChunk)
			{
				return nullptr;
			}

			parsing::Chunk* hshd = digiChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
			if (!hshd)
			{
				return nullptr;
			}
			parsing::HSHD_Chunk* hshdData = hshd->GetData().dataAs<parsing::HSHD_Chunk>();

			std::unique_ptr<SongResource> songResource = std::make_unique<SongResource>();
			songResource->SetName(std::to_string(sgenData->id));
			songResource->SetDataChunk(digiChunk->TryFindChild(parsing::SDAT_CHUNK_ID));
			songResource->SetSampleRate(hshdData->sampleRate);

			return songResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::DIGI_CHUNK_ID) == 0)
		{
			parsing::Chunk* hshd = a_pChunk->TryFindChild(parsing::HSHD_CHUNK_ID);
			if (!hshd)
			{
				return nullptr;
			}
			parsing::HSHD_Chunk* hshdData = hshd->GetData().dataAs<parsing::HSHD_Chunk>();

			std::unique_ptr<SFXResource> sfxResource = std::make_unique<SFXResource>();
			sfxResource->SetName(a_sFallbackName);
			sfxResource->SetDataChunk(a_pChunk->TryFindChild(parsing::SDAT_CHUNK_ID));
			sfxResource->SetSampleRate(hshdData->sampleRate);

			return sfxResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::LSC2_CHUNK_ID) == 0 || core::chunkcmp(a_pChunk->GetTag(), parsing::LSCR_CHUNK_ID) == 0)
		{
			std::unique_ptr<LocalScriptResource> scriptResource = std::make_unique<LocalScriptResource>();
			scriptResource->SetName(a_sFallbackName);
			scriptResource->SetDataChunk(a_pChunk);

			return scriptResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::SCRP_CHUNK_ID) == 0 || core::chunkcmp(a_pChunk->GetTag(), parsing::ENCD_CHUNK_ID) == 0 || core::chunkcmp(a_pChunk->GetTag(), parsing::EXCD_CHUNK_ID) == 0)
		{
			std::unique_ptr<GlobalScriptResource> scriptResource = std::make_unique<GlobalScriptResource>();
			scriptResource->SetName(a_sFallbackName);
			scriptResource->SetDataChunk(a_pChunk);

			return scriptResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::VERB_CHUNK_ID) == 0)
		{
			std::unique_ptr<VerbScriptResource> scriptResource = std::make_unique<VerbScriptResource>();
			scriptResource->SetName(a_sFallbackName);
			scriptResource->SetDataChunk(a_pChunk);

			return scriptResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::IM00_CHUNK_ID) == 0)
		{
			std::unique_ptr<RoomBackgroundResource> roomBackgroundResource = std::make_unique<RoomBackgroundResource>();
			roomBackgroundResource->SetName(a_sFallbackName);
			roomBackgroundResource->SetDataChunk(a_pChunk);

			return roomBackgroundResource;
		}
		else if (core::chunkcmp(a_pChunk->GetTag(), parsing::IM01_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM02_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM03_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM04_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM05_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM06_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM07_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM08_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM09_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM10_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM11_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM12_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM13_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM14_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM15_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM16_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM17_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM0A_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM0B_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM0C_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM0D_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM0E_CHUNK_ID) == 0 ||
			core::chunkcmp(a_pChunk->GetTag(), parsing::IM0F_CHUNK_ID) == 0
		)
		{
			std::unique_ptr<RoomImageResource> roomImageResource = std::make_unique<RoomImageResource>();
			roomImageResource->SetName(a_sFallbackName);
			roomImageResource->SetDataChunk(a_pChunk);

			return roomImageResource;
		}
		return nullptr;
	}
}