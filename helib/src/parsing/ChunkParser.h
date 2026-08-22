#pragma once

#include "core/Data.h"

namespace humongousexplorer::core
{
	class Data;
}
namespace humongousexplorer::parsing
{
	class Chunk;

	/// <summary>
	/// Parses a single chunk and its children from a data buffer at a given position.
	/// </summary>
	/// <param name="a_Out">The output Chunk to populate with parsed data.</param>
	/// <param name="a_Buf">The buffer containing the raw chunk data.</param>
	/// <param name="a_iPos">The byte offset within the buffer to start parsing from (default 0).</param>
	/// <returns>True if the chunk was parsed successfully, otherwise false.</returns>
	bool ParseChunk(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos = 0);

	/// <summary>
	/// Parses all top-level chunks from an archive data buffer into the output chunk's children.
	/// </summary>
	/// <param name="a_Out">The output Chunk whose children will be populated with parsed top-level chunks.</param>
	/// <param name="a_Buf">The buffer containing the raw archive data.</param>
	/// <returns>True if all top-level chunks were parsed successfully, otherwise false.</returns>
	bool ParseArchive(Chunk& a_Out, const core::Data& a_Buf);
}
