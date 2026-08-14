#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This is a local script. Scripts are always found in the Resource file (*.(A)) and usually inside Rooms (LFLFs).
	// Scripts contain a set of bytecode instructions with arguments following each instruction (usually).
	// Instructions might for instance be "play sound from talk file with offset x).
	// Instructions might also be simple push byte, push dword instructions.
	// This version of the local script is used in Humongous versions lower than 7.
	class LSCR_Chunk : public HumongousHeader
	{};
#pragma pack(pop)
}