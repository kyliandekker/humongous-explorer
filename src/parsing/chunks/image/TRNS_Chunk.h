#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This chunk is a TRNS chunk. It contains information about the room background and seems to store another fill color.
	// TRNS chunks are usually found inside RMDA chunks in Resource files (*.(A)).
	class TRNS_Chunk
	{
		// This seems to be the room transparency.
		uint8_t trns_val = 0;
	};
#pragma pack(pop)
}