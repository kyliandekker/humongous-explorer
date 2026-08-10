#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This is a TALK chunk.
	// TALK chunks have child chunks that will always have an SDAT and HSHD chunk.
	// Occasionally, TALK chunks will have SBNG chunks. NOTE: It is unknown what SBNG chunks are used for.
	// NOTE: It is usually found in the Talk file (*.HE2), but can be found in the Resource file as well (*.(A)).
	struct TALK_Chunk : public ChunkHeader
	{ };
#pragma pack(pop)
}