#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This chunk contains data for images.
	// It has custom encoding from Humongous entertainment and is a headache to understand and explain.
	// The first byte is the encoding and says whether it is transparent or not and what encoding it exactly is (basic, Humongous, etc).
	// The second byte is the fill color of the image.
	// BMAP chunks are usually found inside IMxx chunks in Resource files (*.(A)).
	class BMAP_Chunk
	{
		uint8_t encoding = 0;
		uint8_t fill_color = 0;
	};
#pragma pack(pop)
}