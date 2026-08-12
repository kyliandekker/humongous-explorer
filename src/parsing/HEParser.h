#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "core/Data.h"

namespace humongousexplorer::parsing
{
    constexpr auto CHUNK_ID_SIZE = 4;
    constexpr auto HEADER_SIZE = CHUNK_ID_SIZE + sizeof(uint32_t);

    //---------------------------------------------------------------------
    struct Chunk
    {
        char m_sTag[CHUNK_ID_SIZE] = {};
        core::Data m_Data;           // leaf: Owns its data.
        std::vector<Chunk> m_aChildren; // container: Owns its children.
        Chunk* m_pParent = nullptr;

        size_t ChunkSize() const;
        size_t WholeChunkSize() const;

        Chunk* TryFindChild(const std::string& a_sChunkID);
        Chunk* FindChunkAt(size_t a_iTarget, size_t a_iBase = 0);
        Chunk* GetRoot();
    };

    //---------------------------------------------------------------------
	void ParseChunks(Chunk& a_Out, const unsigned char* a_pBuf, size_t a_iPos = 0);
}
