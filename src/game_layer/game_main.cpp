#include "game_main.hpp"

#include <algorithm>
#include <iostream>

#include <raylib.h>

// This structure holds all of the game data (hence, the name). We make it static so that it is
// private to this translation unit, preventing other files from accessing it willy-nilly.
static struct GameData
{
    float position_x = 100.0f;
    float position_y = 100.0f;
} game_data;

bool init_game()
{
    return true;
}

bool update_game()
{
    constexpr float speed = 200.0f;
    constexpr Color c{0xff, 0x00, 0xc8, 0xff};

    // If we let the frame time get too big, that will cause the game to freeze up and break a lot
    // of systems, such as collision. Therefore we must cap the frame time, here to a maximum of
    // 200 milliseconds.
    const float dt = std::min(GetFrameTime(), 0.2f);

    if (IsKeyDown(KEY_A)) { game_data.position_x -= speed * dt; }
    if (IsKeyDown(KEY_D)) { game_data.position_x += speed * dt; }
    if (IsKeyDown(KEY_W)) { game_data.position_y -= speed * dt; }
    if (IsKeyDown(KEY_S)) { game_data.position_y += speed * dt; }

    DrawRectangle(game_data.position_x, game_data.position_y, 50, 50, c);

    return true;
}

void close_game()
{
    // Word of warning: with the current setup, if the player types ^C in the terminal or kills the
    // process, this function will not be called. That means that the game data will not be saved.
    // Maybe this could be changed with atexit or signal handling?
    std::cout << "\n\nCLOSED!!!\n\n";
}
