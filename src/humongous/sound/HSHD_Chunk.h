#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This is a HSHD chunk. The chunk appears in every chunk that contains sound data.
	// It appears in TALK chunks and DIGI chunks.
	// It describes info about the sound data, such as what the sample rate is.
	struct HSHD_Chunk
	{
	public:
		unsigned char unknown1[2] = {
			0,
			0
		};
		uint16_t unknown2 = 32896;
		uint16_t unknown3 = 65535;
		uint16_t sampleRate = 11025;
		unsigned char unknown4 = 8; // We need to figure out what this is, because unlike the other ones, this one is different every time.
		unsigned char unknown5 = 0;
		unsigned char unknown6[2] = {
			0,
			0
		};
		uint32_t unknown7 = 6747836;
	};
#pragma pack(pop)
}