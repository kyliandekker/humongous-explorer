#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This chunk contains information about the room background such as width and height.
	// It is usually found in a RMDA chunk inside the Resource file (*.(A)).
	class RMHD_Chunk : public HumongousHeader
	{
		uint16_t width = 0;
		uint16_t height = 0;
		uint16_t robjs = 0;
	};
#pragma pack(pop)
}