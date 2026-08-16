#pragma once

#include <cstdint>

#include "core/Data.h"

namespace humongousexplorer::core
{
    class Data;
}
namespace humongousexplorer::parsing
{
    class Chunk;

    //---------------------------------------------------------------------
    bool ParseChunk(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos = 0);
    bool ParseArchive(Chunk& a_Out, const core::Data& a_Buf);
}
