#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// This is a HSHD chunk. The chunk appears in every chunk that contains sound data.
	// It appears in TALK chunks and DIGI chunks.
	// It describes info about the sound data, such as what the sample rate is.
	struct HSHD_Chunk : public ChunkHeader
	{
	public:
		unsigned char unknown1[2] = {
			0,
			0
		};
		uint16_t unknown2 = 32896;
		uint16_t unknown3 = 65535;
		uint16_t sampleRate = 11025;
		unsigned char bitsPerSample = 8;
		unsigned char unknown4[1] = 0;
		unsigned char unknown5[2] = {
			0,
			0
		};
		uint32_t unknown6 = 6747836;
	};
#pragma pack(pop)
}