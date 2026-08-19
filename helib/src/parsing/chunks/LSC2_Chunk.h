#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	/// <summary>
	/// This is a local script used in Humongous version 7 and higher.
	/// Scripts are always found in the Resource file (*.(A)) and usually inside Rooms (LFLFs).
	/// Scripts contain a set of bytecode instructions with arguments following each instruction (usually).
	/// Instructions might for instance be "play sound from talk file with offset x".
	/// Instructions might also be simple push byte, push dword instructions.
	/// </summary>
	class LSC2_Chunk
	{
	public:
		unsigned char* data = nullptr;
	};
#pragma pack(pop)
}