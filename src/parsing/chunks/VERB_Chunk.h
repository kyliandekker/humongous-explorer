#pragma once

#include "humongous/ChunkHeader.h"

namespace humongousexplorer::parsing
{
#pragma pack(push, 1)
	// Verb script apparently.
	class VERB_Chunk : public HumongousHeader
	{
	public:
		unsigned char* data = nullptr;
	};
#pragma pack(pop)
}