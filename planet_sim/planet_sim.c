#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const int screenWidth = 640;
    const int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "Title");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        //Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}