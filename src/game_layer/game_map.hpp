#pragma once

#include <vector>

#include "block.hpp"

struct GameMap
{
    std::vector<Block> map_data;
    int width = 0;
    int height = 0;

    /// Set the map's dimensions to those specified and set all blocks to air.
    void create(int width, int height);

    /// Get the block at the specified location.
    ///
    /// If the given position is out of bounds relative to the current map size, this function will
    /// return `nullptr`. If you are sure that the given position is valid, see `get_block_unsafe`.
    Block *get_block(int x, int y);

    /// Get the block at the specified location.
    ///
    /// This method does not check if the given position is in bounds, so it has the potential to
    /// return an invalid reference. Use with care! If you are unsure about whether or not the
    /// location passed is valid, consider using the safe alternative `get_block`.
    Block &get_block_unsafe(int x, int y);
};
