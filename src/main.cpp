#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Hello!");
    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.FontGlobalScale = 2; // If we wanted to change the font size, this is how we would do it.

    // constexpr Color c{0xff, 0x00, 0xc8, 0xff};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        rlImGuiBegin();

        // This allows us to dock windows inside of the main viewport, instead of only being able
        // to dock them inside of other windows. The calls to ImGui::PushStyleColor make it so the
        // viewport dock does not cover up the normal content of the window.
        ImGui::PushStyleColor(ImGuiCol_WindowBg, 0x00000000);
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, 0x00000000);
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        ImGui::PopStyleColor(2);

        static bool c = false;
        static int input_int = 0;
        static float floats[3]{};
        static float vslider = 0.0f;
        static float color = 0.0f;
        static float color2 = 0.0f;

        ImGui::Begin("test");
        ImGui::Checkbox("Program the game", &c);
        ImGui::RadioButton("An active radio button", true);
        ImGui::RadioButton("An inactive radio button", false);
        ImGui::InputInt("Type a number", &input_int);
        ImGui::DragFloat3("Dragging 3 floats", floats);
        ImGui::VSliderFloat("A vslider", ImVec2(10, 10), &vslider, 0.0f, 1.0f);
        ImGui::ColorEdit3("Color edit!", &color);
        ImGui::ColorPicker4("A color picker", &color2);
        ImGui::BeginChild("Hello from the child");
        ImGui::ProgressBar(0.67f);
        ImGui::CollapsingHeader("Collapsing header?");
        ImGui::TreeNode("A tree node");
        ImGui::EndChild();
        ImGui::End();

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

    return 0;
}
