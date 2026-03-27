#pragma once

#include <raylib.h>

/// The asset manager.
///
/// The way that assets are handled in this game is dirt simple. It just holds all of the assets
/// that we need on the stack, loading them all when needed. Since there are not going to be that
/// many assets for the game and they will be required for the entire duration of the program, this
/// is the best approach.
struct AssetManager
{
    Texture2D dirt;
    Texture2D texture_atlas;
    Texture2D frame;

    void load_all();
};
