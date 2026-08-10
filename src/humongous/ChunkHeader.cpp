#include "./ChunkHeader.h"

#include <cstring>

#include "core/Memory.h"

namespace humongousexplorer::headers
{
	ChunkHeader::ChunkHeader(const char* a_sChunkID)
	{
		memcpy(chunkID, a_sChunkID, CHUNK_ID_SIZE);
	}

	ChunkHeader::ChunkHeader(const char* a_sChunkID, uint32_t a_iChunkSize, bool a_bBigEndian)
	{
		memcpy(chunkID, a_sChunkID, CHUNK_ID_SIZE);
		SetChunkSize(a_iChunkSize, a_bBigEndian);
	}

    ChunkHeader::ChunkHeader(unsigned char* a_sChunkID, uint32_t a_iChunkSize, bool a_bBigEndian)
    {
		memcpy(chunkID, a_sChunkID, CHUNK_ID_SIZE);
		SetChunkSize(a_iChunkSize, a_bBigEndian);
    }

    ChunkHeader::ChunkHeader(const ChunkHeader& rhs)
	{
		memcpy(chunkID, rhs.chunkID, CHUNK_ID_SIZE);
		memcpy(chunkSize, rhs.chunkSize, sizeof(uint32_t));
	}

	ChunkHeader::ChunkHeader(ChunkHeader* a_pDataBuffer)
	{
		memcpy(chunkID, a_pDataBuffer->chunkID, CHUNK_ID_SIZE);
		memcpy(chunkSize, a_pDataBuffer->chunkSize, sizeof(uint32_t));
	}

	uint32_t ChunkHeader::ChunkSize(bool a_bBigEndian) const
	{
		uint32_t size32 = 0;
		memcpy(&size32, &chunkSize, sizeof(uint32_t));
		if (a_bBigEndian)
		{
			size32 = core::reverseBytesC<uint32_t>(reinterpret_cast<unsigned char*>(&size32));
		}
		return size32;
	}

    uint32_t ChunkHeader::DataSize(bool a_BigEndian) const
    {
        return ChunkSize(a_BigEndian) - sizeof(ChunkHeader);
    }

	void ChunkHeader::SetChunkSize(uint32_t a_iChunkSize, bool a_bBigEndian)
	{
		uint32_t size32 = static_cast<uint32_t>(a_iChunkSize);
		memcpy(chunkSize, reinterpret_cast<unsigned char*>(&size32), sizeof(uint32_t));
		if (a_bBigEndian)
		{
			core::reverseBytes(chunkSize, sizeof(uint32_t));
		}
	}
}