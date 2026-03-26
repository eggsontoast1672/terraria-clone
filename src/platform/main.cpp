#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include "game_main.hpp"

int main()
{
#if PRODUCTION_BUILD
    SetTraceLogLevel(LOG_NONE);
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Hello!");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.FontGlobalScale = 2; // If we wanted to change the font size, this is how we would do it.

    if (!init_game())
    {
        return 1;
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        rlImGuiBegin();

        // This allows us to dock windows inside of the main viewport, instead of only being able
        // to dock them inside of other windows. The calls to ImGui::PushStyleColor make it so the
        // viewport dock does not cover up the normal content of the window.
        ImGui::PushStyleColor(ImGuiCol_WindowBg, 0x00000000);
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, 0x00000000);
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        ImGui::PopStyleColor(2);

        if (!update_game())
        {
            CloseWindow();
        }

        static float a = 0.0f;

        // We can have more than one window by calling ImGui::Begin and ImGui::End multiple times!
        ImGui::Begin("Second Window");
        ImGui::Separator();
        ImGui::NewLine();
        ImGui::SliderFloat("slider", &a, 0.0f, 1.0f);
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    // We close out the game and unload resources *after* the window is closed. The unloading
    // process can take a lot of time, and we do not want the window to hang unresponsively while 
    // that is happening.
    close_game();
}
