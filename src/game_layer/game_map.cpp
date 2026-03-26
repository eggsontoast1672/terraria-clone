#include "game_map.hpp"

#include <algorithm>

#include "asserts.hpp"

void GameMap::create(int width, int height)
{
    map_data.resize(width * height);

    this->width = width;
    this->height = height;

    std::fill(map_data.begin(), map_data.end(), Block{Block::Air});
}

Block *GameMap::get_block(int x, int y)
{
    permaAssertCommentDevelopment(map_data.size() == width * height, "map data not initialized");
    if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;
    return &map_data[y * width + x];
}

Block &GameMap::get_block_unsafe(int x, int y)
{
    permaAssertCommentDevelopment(map_data.size() == width * height, "map data not initialized");
    permaAssertCommentDevelopment(x >= 0 && x < width && y >= 0 && y < height, "block position out of bounds");

    return map_data[y * width + x];
}
