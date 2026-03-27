#include "asset_manager.hpp"

#include <raylib.h>

void AssetManager::load_all()
{
    dirt = LoadTexture(RESOURCES_PATH "dirt.png");
    texture_atlas = LoadTexture(RESOURCES_PATH "textures.png");
    frame = LoadTexture(RESOURCES_PATH "frame.png");
}
