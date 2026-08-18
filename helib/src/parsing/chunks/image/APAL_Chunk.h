#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// The APAL chunk is a chunk that contains the palette that each room uses.
	// From my experience, it will always contain 256 colors, each with RGB (so 256 * 3).
	// The APAL chunk is found inside a Resource file (*.(A)).
	class APAL_Chunk
	{
	public:
		// Color palette with 256 colors * rgb.
		unsigned char data[256 * 3];
	};
#pragma pack(pop)
}