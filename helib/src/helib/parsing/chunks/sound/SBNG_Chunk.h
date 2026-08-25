#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	struct SBNGRecord
	{
		uint16_t size;				// Record size, always 14 (LE)
		uint16_t sampleOffset;		// Sample position in SDAT where event triggers (LE)
		uint16_t padding;			// Always 0
		uint16_t eventType;			// Always 6
		uint32_t flags;				// Always 0x21000000 (LE: 00 00 06 00 00 21)
		uint16_t varIndex;			// Sound variable index 1-6 (LE)
		uint16_t padding2;			// Always 0
	};

	// This is an SBNG chunk. It is usually found inside TALK chunks as a child.
	// It adds lip sync to the talk file.
	struct SBNG_Chunk
	{};
#pragma pack(pop)
}