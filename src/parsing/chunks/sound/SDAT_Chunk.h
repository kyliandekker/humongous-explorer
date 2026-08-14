#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This is a SDAT chunk. This chunk appears in every chunk that contains sound information.
	// It appears in TALK chunks and DIGI chunks.
	// It contains uncompressed unsigned 8-bit PCM data.
	struct SDAT_Chunk : public ChunkHeader
	{};
#pragma pack(pop)
}