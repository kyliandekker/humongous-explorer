#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	/// <summary>
	/// The APAL chunk contains the palette that each room uses.
	/// It always contains 256 colors, each with RGB (256 * 3 bytes).
	/// Found inside a Resource file (*.(A)).
	/// </summary>
	class APAL_Chunk
	{
	public:
		unsigned char data[256 * 3];
	};
#pragma pack(pop)
}