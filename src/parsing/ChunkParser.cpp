#include "ChunkParser.h"

#include <cstring>
#include <unordered_map>
#include <string>

#include "parsing/Chunk.h"
#include "parsing/SCHEMA.h"
#include "core/Memory.h"

namespace humongousexplorer::parsing
{
	//---------------------------------------------------------------------
	static uint32_t ReadBE32(const uint8_t* p)
	{
		return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) |
			   (uint32_t(p[2]) << 8)  |  uint32_t(p[3]);
	}

	//---------------------------------------------------------------------
	bool ParseChunks(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos /* = 0 */)
	{
		const unsigned char* data = a_Buf.dataAs<unsigned char>();

		// A chunk always needs an 8-byte header.
		if (a_iPos > a_Buf.size() || a_Buf.size() - a_iPos < 8)
		{
			return false;
		}

		a_Out.SetTag(data + a_iPos);

		const size_t size = ReadBE32(data + a_iPos + 4);

		// Size includes the 8-byte header.
		if (size < 8)
		{
			return false;
		}

		// Entire chunk must fit inside the buffer.
		if (size > a_Buf.size() - a_iPos)
		{
			return false;
		}

		const size_t endPos = a_iPos + size;

		std::string tag(a_Out.GetTag(), CHUNK_ID_SIZE);
		auto it = SCHEMA.find(tag);
		const bool isContainer = it != SCHEMA.end() && !it->second.empty();

		if (isContainer)
		{
			size_t childPos = a_iPos + 8;

			while (childPos < endPos)
			{
				// Remaining bytes aren't enough for a child header.
				if (endPos - childPos < 8)
				{
					return false;
				}

				const size_t childSize = ReadBE32(data + childPos + 4);

				// Child must have a header and fit inside parent.
				if (childSize < 8 || childSize > endPos - childPos)
				{
					return false;
				}

				std::unique_ptr<Chunk> child = std::make_unique<Chunk>();
				child->SetParent(a_Out);
				child->SetEncrypted(a_Out.IsEncrypted());
				child->SetEncryptionKey(a_Out.GetEncryptionKey());

				if (!ParseChunks(*child, a_Buf, childPos))
				{
					return false;
				}

				a_Out.GetChildren().emplace_back(std::move(child));

				childPos += childSize;
			}
		}
		else
		{
			a_Out.SetData(
				core::Data(
					a_Buf.dataAs<char>() + a_iPos + 8,
					size - 8
				)
			);
		}

		return true;
	}
}
