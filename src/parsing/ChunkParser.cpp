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
	static bool ParseSingleChunk(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos)
	{
		const unsigned char* data = a_Buf.dataAs<unsigned char>();

		if (a_iPos > a_Buf.size() || a_Buf.size() - a_iPos < 8)
		{
			return false;
		}

		a_Out.SetTag(data + a_iPos);

		const size_t size = ReadBE32(data + a_iPos + 4);

		if (size < 8)
		{
			return false;
		}

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
				if (endPos - childPos < 8)
				{
					return false;
				}

				const size_t childSize = ReadBE32(data + childPos + 4);

				if (childSize < 8 || childSize > endPos - childPos)
				{
					return false;
				}

				std::unique_ptr<Chunk> child = std::make_unique<Chunk>();
				child->SetParent(a_Out);
				child->SetEncrypted(a_Out.IsEncrypted());
				child->SetEncryptionKey(a_Out.GetEncryptionKey());

				if (!ParseSingleChunk(*child, a_Buf, childPos))
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

	//---------------------------------------------------------------------
	bool ParseChunks(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos /* = 0 */)
	{
		const unsigned char* data = a_Buf.dataAs<unsigned char>();
		size_t currentPos = a_iPos;

		while (currentPos + 8 <= a_Buf.size())
		{
			const size_t chunkSize = ReadBE32(data + currentPos + 4);

			if (chunkSize < 8 || chunkSize > a_Buf.size() - currentPos)
			{
				return false;
			}

			std::unique_ptr<Chunk> child = std::make_unique<Chunk>();
			child->SetParent(a_Out);
			child->SetEncrypted(a_Out.IsEncrypted());
			child->SetEncryptionKey(a_Out.GetEncryptionKey());

			if (!ParseSingleChunk(*child, a_Buf, currentPos))
			{
				return false;
			}

			a_Out.GetChildren().emplace_back(std::move(child));

			currentPos += chunkSize;
		}

		return true;
	}
}
