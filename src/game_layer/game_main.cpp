#include "game_main.hpp"

#include <algorithm>

#include <raylib.h>

#include "asset_manager.hpp"
#include "game_map.hpp"
#include "helpers.hpp"

// This structure holds all of the game data (hence, the name). We make it static so that it is
// private to this translation unit, preventing other files from accessing it willy-nilly.
static struct GameData
{
    GameMap game_map;
    Camera2D camera;
} game_data;

/// The global asset manager.
///
/// This should be kept separate from the game data structure, since we want to keep the assets
/// loaded even if the game data gets reset.
AssetManager asset_manager;

bool init_game()
{
    asset_manager.load_all();

    game_data.game_map.create(30, 10);
    game_data.game_map.get_block_unsafe(0, 0).type = Block::Dirt;
    game_data.game_map.get_block_unsafe(1, 1).type = Block::Grass;
    game_data.game_map.get_block_unsafe(2, 2).type = Block::GoldBlock;
    game_data.game_map.get_block_unsafe(3, 3).type = Block::Glass;
    game_data.game_map.get_block_unsafe(4, 4).type = Block::WoodenPlatform;

    game_data.camera.target = {0.0f, 0.0f};
    game_data.camera.rotation = 0.0f;
    game_data.camera.zoom = 100.0f;

    return true;
}

bool update_game()
{
    constexpr float camera_speed = 7.0f;

    // If we let the frame time get too big, that will cause the game to freeze up and break a lot
    // of systems, such as collision. Therefore we must cap the frame time, here to a maximum of
    // 200 milliseconds.
    const float dt = std::min(GetFrameTime(), 0.2f);

    game_data.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

    if (IsKeyDown(KEY_LEFT)) game_data.camera.target.x -= camera_speed * dt;
    if (IsKeyDown(KEY_RIGHT)) game_data.camera.target.x += camera_speed * dt;
    if (IsKeyDown(KEY_UP)) game_data.camera.target.y -= camera_speed * dt;
    if (IsKeyDown(KEY_DOWN)) game_data.camera.target.y += camera_speed * dt;

    ClearBackground({75, 75, 150, 255});

    BeginMode2D(game_data.camera);

    for (int y = 0; y < game_data.game_map.height; y++)
    {
        for (int x = 0; x < game_data.game_map.width; x++)
        {
            const Block &block = game_data.game_map.get_block_unsafe(x, y);

            if (block.type == Block::Air)
            {
                continue;
            }

            // The order of the block types in the enum corresponds with the order in which the
            // textures in the atlas are set up, so we can just pick which section of the atlas
            // that we want to display this way.
            const Rectangle source = get_texture_atlas(block.type, 0, 32, 32);
            const Rectangle dest{static_cast<float>(x), static_cast<float>(y), 1.0f, 1.0f};

            DrawTexturePro(asset_manager.texture_atlas, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
        }
    }

    EndMode2D();

    return true;
}

void close_game()
{
    // Word of warning: with the current setup, if the player types ^C in the terminal or kills the
    // process, this function will not be called. That means that the game data will not be saved.
    // Maybe this could be changed with atexit or signal handling?
}
