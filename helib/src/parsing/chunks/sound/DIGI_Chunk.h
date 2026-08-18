#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This is a DIGI chunk. DIGI chunks are found inside the Song file (*.HE4), but can be found in the Resource file as well (*.(A)).
	// DIGI chunks usually will have soundtrack data, but if they are stored inside the Resource file, they will be SFX.
	// DIGI chunks have child chunks and will always have an SDAT and HSHD chunk.
	struct DIGI_Chunk : public ChunkHeader
	{};
#pragma pack(pop)
}