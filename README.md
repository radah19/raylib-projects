Just my Raylib projects 😸 most code is taken from tutorials
## Notes to Self for Setup
Use this for creating REAL Raylib projects: https://github.com/raysan5/raylib-project-creator
Otherwise create an individual C file with this starter code:
```c
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

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```
These links have quick start templates/guides for developing on VS 2022:
- https://github.com/raysan5/raylib/wiki/Working-on-Windows
- https://github.com/raylib-extras/raylib-quickstart

These videos made it easy to do:
- https://www.youtube.com/watch?v=O96m52ZxQcw
- https://www.youtube.com/watch?v=xRMN__GECt8