#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// This will always be inside an LFLF chunk and contains the room background.
	// RMIM chunks have child chunks that will usually be an RMIH chunk and an IMxx chunk with usually a BMAP inside the IMxx.
	class RMIM_Chunk
	{ };
#pragma pack(pop)
}