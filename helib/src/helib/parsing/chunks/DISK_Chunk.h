#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	/// <summary>
	/// The DISK chunk describes the number of rooms and has the number of rooms in length of data right after.
	/// The DISK chunk is found inside an Index file (*.HE0).
	/// </summary>
	class DISK_Chunk
	{
	public:
		uint8_t num_rooms = 0;
		unsigned char* data = nullptr;
	};
#pragma pack(pop)
}