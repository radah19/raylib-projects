#include "raylib.h"

int main(void) {
    const int screenWidth = 640;
    const int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "Title");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        //Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        // fuck you
        EndDrawing();
    }

    CloseWindow();
    return 0;
}