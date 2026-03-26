#pragma once

#include <cstdint>

struct Block
{
    enum Type : std::uint16_t
    {
        Air,
        Dirt,
        Count,
    };

    Block::Type type{};
};
