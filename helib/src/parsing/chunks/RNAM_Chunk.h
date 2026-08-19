#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	/// <summary>
	/// The RNAM chunk contains the names for each room.
	/// Each room starts with a room index (for instance room 0), and is then followed by a null-terminated string that is the name of the room.
	/// After the null value, a new room follows with the same setup (index first, null-terminated string).
	/// The number of rooms is obtained from the DISK chunk that precedes it.
	/// </summary>
	class RNAM_Chunk
	{
	};
#pragma pack(pop)
}