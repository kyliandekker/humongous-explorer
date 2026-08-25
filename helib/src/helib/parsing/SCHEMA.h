#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace humongousexplorer::parsing
{
	//---------------------------------------------------------------------
	// NOTE: We never actually check specifically for chunks.
	// We just keep track of what chunks are containers and what chunks are pure data.
	// The detail here is not necessary, but it might be handy for later to know what chunks appear in what container.
	const extern std::unordered_map<std::string_view, std::vector<std::string_view>> SCHEMA;
}