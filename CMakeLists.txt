cmake_minimum_required(VERSION 3.14)
project(SortingVisualizer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0135 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Automatically download and compile Raylib for Windows, macOS, and Linux
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)

# add your source .cpp files here
add_executable(sorting_visualizer 
    src/main.cpp
)

target_include_directories(sorting_visualizer PRIVATE include)

# Link Raylib to your executable
target_link_libraries(sorting_visualizer PRIVATE raylib)