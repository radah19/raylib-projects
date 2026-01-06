Just my Raylib projects 😸 most code is taken from tutorials
# Notes to Self
Use this for creating REAL Raylib projects: https://github.com/raysan5/raylib-project-creator
Otherwise create a separate C file with this starter code:
```c
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

```
