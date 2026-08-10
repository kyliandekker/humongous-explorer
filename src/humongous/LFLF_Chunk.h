#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This signifies the start of a room.
	// Rooms contain scripts, sfx, costumes, backgrounds, etc.
	class LFLF_Chunk : public HumongousHeader
	{ };
#pragma pack(pop)
}