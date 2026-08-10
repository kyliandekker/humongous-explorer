#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This is a global script. Scripts are always found in the Resource file (*.(A)).
	// Scripts contain a set of bytecode instructions with arguments following each instruction (usually).
	// Instructions might for instance be "play sound from talk file with offset x).
	// Instructions might also be simple push byte, push dword instructions.
	class ENCD_Chunk : public HumongousHeader
	{};
#pragma pack(pop)
}