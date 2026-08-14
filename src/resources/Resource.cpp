#include "./Resource.h"

#include "parsing/ChunkParser.h"

#include "resources/ResourceType.h"

#include "parsing/Chunk.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	uint16_t SoundResource::GetSampleRate() const
	{
		return m_iSampleRate;
	}

	//---------------------------------------------------------------------
	void SoundResource::SetSampleRate(uint16_t a_iSampleRate)
	{
		m_iSampleRate = a_iSampleRate;
	}

	//---------------------------------------------------------------------
	core::Data SoundResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}
	
	//---------------------------------------------------------------------
	void SoundResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	size_t SoundResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return 0;
		}
		return m_pDataChunk->ChunkSize();
	}

	//---------------------------------------------------------------------
	std::string SoundResource::GetDurationStr()
	{
		if (!m_pDataChunk)
		{
			return "";
		}

		size_t bytesPerSecond =
			(size_t)m_iSampleRate * 1 * (8 / 8);

		size_t totalMs =
			(m_pDataChunk->GetData().size() * 1000) / bytesPerSecond;

		// size_t hours = totalMs / 3600000;
		size_t minutes = (totalMs % 3600000) / 60000;
		size_t seconds = (totalMs % 60000) / 1000;
		size_t ms = totalMs % 1000;

		static char buffer[32];
		snprintf(buffer, sizeof(buffer),
			"%zu:%02zu.%03zu",
			minutes, seconds, ms);

		return buffer;
	}

	//---------------------------------------------------------------------
	SongResource::SongResource() : SoundResource()
	{
		m_eResourceType = resources::ResourceType::Song;
	}

	//---------------------------------------------------------------------
	TalkResource::TalkResource() : SoundResource()
	{
		m_eResourceType = resources::ResourceType::Talkie;
	}

	//---------------------------------------------------------------------
	core::Data TalkResource::GetLipSyncData() const
	{
		if (!m_pLipSyncChunk)
		{
			return core::Data();
		}
		return m_pLipSyncChunk->GetData();
	}

	//---------------------------------------------------------------------
	void TalkResource::SetLipSyncChunk(parsing::Chunk* a_pChunk)
	{
		m_pLipSyncChunk = a_pChunk;
	}
}