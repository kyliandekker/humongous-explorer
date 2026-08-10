#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::headers
{
#pragma pack(push, 1)
	// The RNAM chunk contains the names for each room. It does not store the number of rooms, however.
	// I get the number of rooms from the DISK room that precedes it.
	// Each room starts with a room index (for instance room 0), and is then followed by a null-terminated string that is the name of the room.
	// After the null value, a new room follows with the same setup (index first, null-terminated string).
	class RNAM_Chunk : public HumongousHeader
	{
	};
#pragma pack(pop)
}