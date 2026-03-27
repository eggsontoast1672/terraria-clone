cmake_minimum_required(VERSION 3.10)

project(TerrariaClone LANGUAGES CXX)

set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.7-docking
)

set(BUILD_SHARED_LIBS OFF)            # Link statically against raylib
set(CMAKE_POLICY_VERSION_MINIMUM 3.5) # Disable version warning

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0
)

FetchContent_Declare(
  rlimgui
  GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
  GIT_TAG a59676ae038cfbd117f53aa398cdc50433d4f003 # standard int
)

FetchContent_MakeAvailable(imgui raylib rlimgui)

# Since ImGui does not have any build system set up, we just create a library
# here after CMake fetches the code. Same goes for the rlImGui library, see the
# code for that below.
add_library(
  imgui
  ${imgui_SOURCE_DIR}/imconfig.h
  ${imgui_SOURCE_DIR}/imgui.h
  ${imgui_SOURCE_DIR}/imgui_internal.h
  ${imgui_SOURCE_DIR}/imstb_rectpack.h
  ${imgui_SOURCE_DIR}/imstb_textedit.h
  ${imgui_SOURCE_DIR}/imstb_truetype.h
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# Setup code for rlImGui. Note that we need to manually create a CMake build
# for this library as well, since it does not have its own build. Additionally,
# it needs access to the imgui and raylib headers in order to work properly, so
# we will link in those libraries.
add_library(
  rlimgui
  ${rlimgui_SOURCE_DIR}/rlImGui.h
  ${rlimgui_SOURCE_DIR}/rlImGuiColors.h
  ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)

target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(rlimgui PRIVATE imgui raylib)

add_executable(
  terraria-clone
  src/game_layer/asset_manager.hpp
  src/game_layer/asset_manager.cpp
  src/game_layer/game_main.hpp
  src/game_layer/game_main.cpp
  src/game_layer/game_map.hpp
  src/game_layer/game_map.cpp
  src/game_layer/helpers.hpp
  src/game_layer/helpers.cpp
  src/platform/asserts.hpp
  src/platform/asserts.cpp
  src/platform/main.cpp
)

set_property(TARGET terraria-clone PROPERTY CXX_STANDARD 17)
set_property(TARGET terraria-clone PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET terraria-clone PROPERTY CXX_EXTENSIONS OFF)

target_compile_options(terraria-clone PRIVATE -mavx2 -mfma)
target_include_directories(terraria-clone PRIVATE src/game_layer src/platform)
target_link_libraries(terraria-clone PRIVATE imgui raylib rlimgui)

if(PRODUCTION_BUILD)
  target_compile_definitions(terraria-clone PUBLIC PRODUCTION_BUILD=1) 
  target_compile_definitions(terraria-clone PUBLIC RESOURCES_PATH="./resources/") 
else()
  target_compile_definitions(terraria-clone PUBLIC PRODUCTION_BUILD=0) 
  target_compile_definitions(terraria-clone PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
endif()
