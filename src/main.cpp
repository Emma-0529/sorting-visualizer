#include "raylib.h"

int main() {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Sorting Visualizer - Base Setup");
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        
        // Draw phase
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            DrawText("Success", 220, 250, 20, BLACK);
            
        EndDrawing();
    }

    // Clean up
    CloseWindow();

    return 0;
}