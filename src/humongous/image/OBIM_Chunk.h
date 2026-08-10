#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// The OBIM chunk contains room images.
	// Each OBIM chunk can contain multiple images. They will always be IMxx.
	// OBIM chunks always have a IMHD chunk, but will not always contain images.
	// I think Humongous sometimes forgot to put images inside but did have the data there for them.
	// The OBIM chunk is found inside a Resource file (*.(A)).
	class OBIM_Chunk : public HumongousHeader
	{ };
#pragma pack(pop)
}