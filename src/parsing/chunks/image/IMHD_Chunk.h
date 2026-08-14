#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This chunk contains information about the room images such as width and height and where on the screen they should appear.
	// It is usually found in a OBIM chunk inside the Resource file (*.(A)).
	// It is used for multiple images.
	class IMHD_Chunk : public HumongousHeader
	{
		uint16_t id = 0;
		uint16_t num_imnn = 0;
		uint16_t nums_zpnn = 0;
		uint8_t flags = 0;
		uint8_t unknown = 0;
		uint16_t x = 0;
		uint16_t y = 0;
		uint16_t width = 0;
		uint16_t height = 0;
	};
#pragma pack(pop)
}