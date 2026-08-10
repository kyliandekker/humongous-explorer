#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This chunk contains data for images.
	// It has custom encoding from Humongous entertainment and is a headache to understand and explain.
	// It is divided in strips and each strip has the same kind of setup as a BMAP chunk with encoding and fill color stored.
	// SMAP chunks are usually found inside IMxx chunks in Resource files (*.(A)).
	class SMAP_Chunk : public HumongousHeader
	{};
#pragma pack(pop)
}