#include "helpers.hpp"

#include <raylib.h>

Rectangle get_texture_atlas(int x, int y, int cell_size_pixels_x, int cell_size_pixels_y)
{
    return Rectangle{
        static_cast<float>(x) * cell_size_pixels_x,
        static_cast<float>(y) * cell_size_pixels_y,
        static_cast<float>(cell_size_pixels_x),
        static_cast<float>(cell_size_pixels_y),
    };
}
