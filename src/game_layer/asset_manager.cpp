#include "asset_manager.hpp"

#include <raylib.h>

void AssetManager::load_all()
{
    dirt = LoadTexture(RESOURCES_PATH "dirt.png");
}
