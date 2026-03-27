#include "game_main.hpp"

#include <algorithm>
#include <cmath>

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

    game_data.game_map.create(700, 500);

    std::fill(game_data.game_map.map_data.begin(), game_data.game_map.map_data.end(), Block{Block::WoodPlank});

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

static Vector2 get_screen_dimensions()
{
    return {
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight()),
    };
}

/// Compute the visible region of the map.
///
/// Rendering every object in the game world begins to be painfully slow once there are tons of
/// them. Since only a small portion of the world is visible at a time, this function proves the
/// visible portion of the world so that only objects in that region are rendered. Note that this
/// function clamps the viewport to the size of the map for pragmatics, since this data is only
/// used to figure out how much of the map to render.
static Rectangle get_viewport(Camera2D camera)
{
    const Vector2 top_left_view = GetScreenToWorld2D({0.0f, 0.0f}, game_data.camera);
    const Vector2 bottom_right_view = GetScreenToWorld2D(get_screen_dimensions(), game_data.camera);

    // These offsets ensure that the world does not appear cut off.
    float start_x_view = std::floor(top_left_view.x - 1.0f);
    float start_y_view = std::floor(top_left_view.y - 1.0f);
    float end_x_view = std::ceil(bottom_right_view.x + 1.0f);
    float end_y_view = std::ceil(bottom_right_view.y + 1.0f);

    start_x_view = std::clamp(start_x_view, 0.0f, game_data.game_map.width - 1.0f);
    start_y_view = std::clamp(start_y_view, 0.0f, game_data.game_map.height - 1.0f);
    end_x_view = std::clamp(end_x_view, 0.0f, game_data.game_map.width - 1.0f);
    end_y_view = std::clamp(end_y_view, 0.0f, game_data.game_map.height - 1.0f);

    return {start_x_view, start_y_view, end_x_view, end_y_view};
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

    // This code converts the mouse's current position in screen space coordinates to an absolute
    // world position, according to the camera data. With this, we will be able to highlight the
    // block that is currently selected.
    const Vector2 world_pos = GetScreenToWorld2D(GetMousePosition(), game_data.camera);
    const float block_x = std::floor(world_pos.x);
    const float block_y = std::floor(world_pos.y);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Block *const block = game_data.game_map.get_block(block_x, block_y);

        if (block != nullptr)
        {
            block->type = Block::Air;
        }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Block *const block = game_data.game_map.get_block(block_x, block_y);

        if (block != nullptr)
        {
            // For now, we can only place gold blocks, but that will change.
            block->type = Block::GoldBlock;
        }
    }

    ClearBackground({75, 75, 150, 255});
    BeginMode2D(game_data.camera);

    const Rectangle viewport = get_viewport(game_data.camera);

    for (int y = viewport.y; y < viewport.height; y++)
    {
        for (int x = viewport.x; x < viewport.width; x++)
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

    DrawTexturePro(
        asset_manager.frame,
        {0.0f, 0.0f, static_cast<float>(asset_manager.frame.width), static_cast<float>(asset_manager.frame.height)},
        {block_x, block_y, 1.0f, 1.0f},
        {0.0f, 0.0f}, 0.0f, WHITE);

    EndMode2D();

    return true;
}

void close_game()
{
    // Word of warning: with the current setup, if the player types ^C in the terminal or kills the
    // process, this function will not be called. That means that the game data will not be saved.
    // Maybe this could be changed with atexit or signal handling?
}
