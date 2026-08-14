#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// The DISK chunk seems to describe the number of rooms and has the number of rooms in length of data right after.
	// What this data tells me exactly is really unclear to me.
	// The DISK chunk is found inside an Index file (*.HE0).
	class DISK_Chunk
	{
	public:
		uint8_t num_rooms = 0;
		unsigned char* data = nullptr;
	};
#pragma pack(pop)
}