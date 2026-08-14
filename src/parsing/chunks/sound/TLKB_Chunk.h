#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This is the root chunk of a Talk file (*.HE2).
	// Stands for Talk Bank.
	// It holds no actual data and is just used to identify the type of archive and the size of the archive.
	struct TLKB_Chunk : public ChunkHeader
	{ };
#pragma pack(pop)
}