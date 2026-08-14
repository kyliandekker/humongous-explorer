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
    bool ParseChunks(Chunk& a_Out, const core::Data& a_Buf, size_t a_iPos = 0);
}
