#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This chunk is a pointer chunk that appears several times at the start of the Song file (*.HE4).
	// It says where the song related to the SGEN appears and what the size is.
	// SGENs are not always in order of appearance.
	struct SGEN_Chunk : public ChunkHeader
	{
		uint32_t id = 0; // For some reason it does not start at 0 most of the time.
		uint32_t song_pos = 0; // Direct pointer to the DIGI header of the song.
		uint32_t song_size = 0; // This is the ENTIRE DIGI chunk.
		uint8_t padding = 0;
	};
#pragma pack(pop)
}