/*

        Code based off Ian Millington's Cyclone Physics Engine - https://github.com/idmillington/cyclone-physics/tree/master

*/

#include "raylib.h"
#include <stdlib.h>

// ---------------------- real typedef definition -----------------------------
// Done so that, in case precision of variables needs to be modified, it can
// be quickly modified in the real typedef

#if 1
    // Single Precision Mode
#define SINGLE_PRECISION

/**
* Defines a real number precision. Cyclone can be compiled in
* single or double precision versions. By default single precision is
* provided.
*/
typedef float real;
#else
    // Double Precision Mode
#define DOUBLE_PRECISION
typedef double real;
#endif
// ----------------------------------------------------------------------------



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