#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This is the root chunk of a Resource file (*.(A)).
	class LECF_Chunk : public ChunkHeader
	{ };
#pragma pack(pop)
}