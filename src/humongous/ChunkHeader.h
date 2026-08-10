#pragma once

#include "humongous/Defines.h"

constexpr bool BIG_ENDIAN_DEFAULT = true;
namespace humongousexplorer::headers
{
#pragma pack(push, 1)
		/*
		 * WHAT IS THIS FILE?
		 * Every humongous chunk starts the same, with a header containing the name and chunk size.
		 */
		struct ChunkHeader
		{
			ChunkHeader() = default;
			ChunkHeader(const char* a_sChunkID);
			ChunkHeader(const char* a_sChunkID, uint32_t a_iChunkSize = 0, bool a_bBigEndian = BIG_ENDIAN_DEFAULT);
			ChunkHeader(unsigned char* a_sChunkID, uint32_t a_iChunkSize = 0, bool a_bBigEndian = BIG_ENDIAN_DEFAULT);
			ChunkHeader(const ChunkHeader& rhs);
			ChunkHeader(ChunkHeader* a_pDataBuffer);

			unsigned char chunkID[CHUNK_ID_SIZE] = {};
			unsigned char chunkSize[sizeof(uint32_t)] = {}; // LE.

			uint32_t ChunkSize(bool a_bBigEndian = BIG_ENDIAN_DEFAULT) const;
			uint32_t DataSize(bool a_bBigEndian = BIG_ENDIAN_DEFAULT) const;

			void SetChunkSize(uint32_t a_iChunkSize, bool a_bBigEndian = BIG_ENDIAN_DEFAULT);
		};
#pragma pack(pop)
}