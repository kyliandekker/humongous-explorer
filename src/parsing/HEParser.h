#pragma once

#include <cstdint>
#include <vector>

namespace humongousexplorer::parsing
{
	struct Chunk
	{
		char tag[4] = {};
		uint32_t offset = 0;
		uint32_t size = 0;
		const uint8_t* data = nullptr;
		std::vector<Chunk> children;
	};

	void ParseChunks(Chunk& out, const uint8_t* buf, size_t len, size_t pos = 0);
}
