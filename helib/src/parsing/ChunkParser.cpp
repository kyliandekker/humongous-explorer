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
	// Header is CHUNK_ID + uint32_t size.
	constexpr size_t CHUNK_HEADER_SIZE = CHUNK_ID_SIZE + sizeof(uint32_t);

	//---------------------------------------------------------------------
	bool ParseChunk(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos /* = 0 */)
	{
		const unsigned char* data = a_Buf.dataAs<unsigned char>();

		if (a_iPos > a_Buf.size() || a_Buf.size() - a_iPos < CHUNK_HEADER_SIZE)
		{
			return false;
		}

		memcpy(a_Out.m_sTag, data + a_iPos, CHUNK_ID_SIZE);

		const size_t size = core::ReadBE32(data + a_iPos + CHUNK_ID_SIZE);

		if (size < CHUNK_HEADER_SIZE || size > a_Buf.size() - a_iPos)
		{
			return false;
		}

		std::string tag(a_Out.m_sTag, CHUNK_ID_SIZE);
		auto it = SCHEMA.find(tag);
		const bool isContainer = it != SCHEMA.end() && !it->second.empty();

		if (isContainer)
		{
			size_t childPos = a_iPos + CHUNK_HEADER_SIZE;
			const size_t endPos = a_iPos + size;

			while (childPos < endPos)
			{
				if (endPos - childPos < CHUNK_HEADER_SIZE)
				{
					return false;
				}

				std::unique_ptr<Chunk>& child =
					a_Out.GetChildren().emplace_back(std::make_unique<Chunk>());

				child->SetParent(a_Out);

				const size_t childSize = core::ReadBE32(data + childPos + CHUNK_ID_SIZE);

				if (childSize < CHUNK_HEADER_SIZE || childSize > endPos - childPos)
				{
					return false;
				}

				if (!ParseChunk(*child, a_Buf, childPos))
				{
					return false;
				}

				childPos += childSize;
			}
		}
		else if (size > CHUNK_HEADER_SIZE)
		{
			a_Out.SetData(core::Data(
				a_Buf.dataAs<char>() + a_iPos + CHUNK_HEADER_SIZE,
				size - CHUNK_HEADER_SIZE
			));
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool ParseArchive(Chunk& a_Out, const core::Data& a_Buf)
	{
		const unsigned char* data = a_Buf.dataAs<unsigned char>(); 
		
		if (a_Buf.size() < CHUNK_HEADER_SIZE) 
		{ 
			return false; 
		} 
		
		size_t currentPos = 0; 
		
		while (currentPos < a_Buf.size())
		{ 
			if (a_Buf.size() - currentPos < CHUNK_HEADER_SIZE) 
			{ 
				return false; 
			} 
			const size_t chunkSize = core::ReadBE32(data + currentPos + CHUNK_ID_SIZE); 
			if (chunkSize < CHUNK_HEADER_SIZE || chunkSize > a_Buf.size() - currentPos)
			{ 
				return false; 
			} 
			auto& child = a_Out.GetChildren().emplace_back(std::make_unique<Chunk>()); 
			child->SetParent(a_Out); 
			
			if (!ParseChunk(*child, a_Buf, currentPos)) 
			{ 
				return false; 
			} 
			currentPos += chunkSize; 
		} 
		
		return true;
	}
}
