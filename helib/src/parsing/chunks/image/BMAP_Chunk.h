#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	/// <summary>
	/// This chunk contains data for images using Humongous custom encoding.
	/// The first byte is the encoding and says whether it is transparent or not and what encoding it exactly is.
	/// The second byte is the fill color of the image.
	/// BMAP chunks are usually found inside IMxx chunks in Resource files (*.(A)).
	/// </summary>
	class BMAP_Chunk
	{
	public:
		uint8_t encoding = 0;
		uint8_t fillColor = 0;
	};
#pragma pack(pop)
}