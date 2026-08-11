#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "core/Data.h"

namespace humongousexplorer::parsing
{
    constexpr auto CHUNK_ID_SIZE = 4;

    struct Chunk
    {
        char m_sTag[CHUNK_ID_SIZE] = {};
        core::Data m_Data;           // leaf: Owns its data.
        std::vector<Chunk> m_aChildren; // container: Owns its children.
        Chunk* m_pParent = nullptr;

        size_t ChunkSize() const;

        Chunk* TryFindChild(const std::string& a_sChunkID);
    };

	void ParseChunks(Chunk& a_Out, const unsigned char* a_pBuf, size_t a_iPos = 0);
}
