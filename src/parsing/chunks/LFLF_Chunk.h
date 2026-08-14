#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This signifies the start of a room.
	// Rooms contain scripts, sfx, costumes, backgrounds, etc.
	class LFLF_Chunk : public HumongousHeader
	{ };
#pragma pack(pop)
}