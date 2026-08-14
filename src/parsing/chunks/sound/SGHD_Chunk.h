#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This chunk appears at the start of the Song file (*.HE4), and just describes the number of songs that are in the file.
	struct SGHD_Chunk : public ChunkHeader
	{
		uint32_t num_of_songs = 0;
		unsigned char unknown1[28]; // Really unsure why this is even here. This is just 28 bytes of padding that is always 0.
	};
#pragma pack(pop)
}