#pragma once

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	/// <summary>
	/// This is a global script. Scripts are always found in the Resource file (*.(A)).
	/// Scripts contain a set of bytecode instructions with arguments following each instruction (usually).
	/// Instructions might for instance be "play sound from talk file with offset x".
	/// Instructions might also be simple push byte, push dword instructions.
	/// </summary>
	class SCRP_Chunk
	{};
#pragma pack(pop)
}