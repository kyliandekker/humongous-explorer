#include "ChunkParser.h"

#include <cstring>
#include <unordered_map>
#include <string>

#include "parsing/Chunk.h"
#include "parsing/SCHEMA.h"
#include "core/Memory.h"
#include "file/file_abstractions.h"

namespace humongousexplorer::parsing
{
	//---------------------------------------------------------------------
	static uint32_t ReadBE32(const uint8_t* p)
	{
		return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) |
			   (uint32_t(p[2]) << 8)  |  uint32_t(p[3]);
	}

    //---------------------------------------------------------------------
    struct ChunkHeader
    {
        char m_sTag[CHUNK_ID_SIZE] = {};
        uint32_t m_iSize;
    };

	//---------------------------------------------------------------------
    bool ParseChunk(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos /* = 0 */)
    {
        const unsigned char* data = a_Buf.dataAs<unsigned char>();

        if (a_iPos > a_Buf.size() || a_Buf.size() - a_iPos < 8)
        {
            return false;
        }

        memcpy(a_Out.m_sTag, data + a_iPos, CHUNK_ID_SIZE);

        const size_t size = ReadBE32(data + a_iPos + CHUNK_ID_SIZE);

        if (size < sizeof(ChunkHeader) || size > a_Buf.size() - a_iPos)
        {
            return false;
        }

        std::string tag(a_Out.m_sTag, CHUNK_ID_SIZE);
        auto it = SCHEMA.find(tag);
        const bool isContainer = it != SCHEMA.end() && !it->second.empty();

        if (isContainer)
        {
            size_t childPos = a_iPos + sizeof(ChunkHeader);
            const size_t endPos = a_iPos + size;

            while (childPos < endPos)
            {
                if (endPos - childPos < sizeof(ChunkHeader))
                {
                    return false;
                }

                std::unique_ptr<Chunk>& child =
                    a_Out.GetChildren().emplace_back();

                child = std::make_unique<Chunk>();
                child->SetParent(a_Out);

                const size_t childSize = ReadBE32(data + childPos + CHUNK_ID_SIZE);

                if (childSize < sizeof(ChunkHeader) || childSize > endPos - childPos)
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
        else if (size > sizeof(ChunkHeader))
        {
            a_Out.SetData(core::Data(
                a_Buf.dataAs<char>() + a_iPos + sizeof(ChunkHeader),
                size - sizeof(ChunkHeader)
            ));
        }

        return true;
    }
}
