#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

// Code from https://thepythoncode.com/article/make-a-planet-simulator-using-pygame-in-python

typedef struct {
    Vector2 pos;
    Vector2 delta; // (0,0) by default

    int radius;
    bool imoveable; // false by default
    bool eatable; // defines whether the planet can be eaten by others, eek!
    Color col;
} Planet;

void process(int index, Planet* planets, int planetsAmn, Vector2* lines, int linePointAmn) {
    // Calculates where the provided planet should go every frame
    Planet* p = &planets[index];

    if (!p->imoveable) {

        // Update Position
        for (int i = 0; i < planetsAmn; i++) {
            if (index != i) {
                // If planet can be eaten
                if (p->eatable) {
                    if (Vector2Distance(p->pos, planets[i].pos) < planets[i].radius + p->radius) {
                        planets[i].radius += p->radius;
                        p->radius = 0; // Lazy deletion
                    }
                }

                Vector2 vectorDiff;
                vectorDiff.x = planets[i].pos.x - p->pos.x;
                vectorDiff.y = planets[i].pos.y - p->pos.y;
                Vector2 normalizedVec = Vector2Normalize(vectorDiff);

                float dir_from_obj_x = normalizedVec.x * 0.01 * (planets[i].radius / 10.0);
                p->delta.x += dir_from_obj_x;

                float dir_from_obj_y = normalizedVec.y * 0.01 * (planets[i].radius / 10.0);
                p->delta.y += dir_from_obj_y;
            }
        }

        p->pos.x += p->delta.x;
        p->pos.y += p->delta.y;

        // Update my trail
        for (int i = 0; i < linePointAmn - 1; i++) {
            lines[i + index * linePointAmn] = lines[i + 1 + index * linePointAmn];
        }
        lines[linePointAmn - 1 + index * linePointAmn] = planets[index].pos;
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 800;

    // Ugly aah Variable Initialization
    Planet* planets = 0;
    int planetsAmn = 0;
    int key = 2;

    switch (key) {
    case 0: { // Sun with two opposing planets
        planetsAmn = 3;
        planets = malloc(sizeof(Planet) * planetsAmn);

        Planet* sun = &planets[0];
        sun->pos.x = 400;
        sun->pos.y = 400;
        sun->delta.x = 0;
        sun->delta.y = 0;
        sun->radius = 50;
        sun->imoveable = true;
        sun->eatable = false;
        sun->col = YELLOW;

        Planet* toad = &planets[1];
        toad->pos.x = 400;
        toad->pos.y = 200;
        toad->delta.x = 3;
        toad->delta.y = 0;
        toad->radius = 10;
        toad->imoveable = false;
        toad->eatable = true;
        toad->col = GREEN;

        Planet* badoum = &planets[2];
        badoum->pos.x = 400;
        badoum->pos.y = 600;
        badoum->delta.x = -3;
        badoum->delta.y = 0;
        badoum->radius = 10;
        badoum->imoveable = false;
        badoum->eatable = true;
        badoum->col = BLUE;

        break;
    }
    case 1: { // Sun with 4 opposing planets
        planetsAmn = 5;
        planets = malloc(sizeof(Planet) * planetsAmn);

        Planet* sun = &planets[0];
        sun->pos.x = 400;
        sun->pos.y = 400;
        sun->delta.x = 0;
        sun->delta.y = 0;
        sun->radius = 50;
        sun->imoveable = true;
        sun->eatable = false;
        sun->col = YELLOW;

        Planet* toad = &planets[1];
        toad->pos.x = 400;
        toad->pos.y = 200;
        toad->delta.x = 3;
        toad->delta.y = 0;
        toad->radius = 10;
        toad->imoveable = false;
        toad->eatable = true;
        toad->col = GREEN;

        Planet* badoum = &planets[2];
        badoum->pos.x = 400;
        badoum->pos.y = 600;
        badoum->delta.x = -3;
        badoum->delta.y = 0;
        badoum->radius = 10;
        badoum->imoveable = false;
        badoum->eatable = true;
        badoum->col = BLUE;

        Planet* tito = &planets[3];
        tito->pos.x = 200;
        tito->pos.y = 400;
        tito->delta.x = 0;
        tito->delta.y = -3;
        tito->radius = 10;
        tito->imoveable = false;
        tito->eatable = true;
        tito->col = RED;

        Planet* dongo = &planets[4];
        dongo->pos.x = 600;
        dongo->pos.y = 400;
        dongo->delta.x = 0;
        dongo->delta.y = 3;
        dongo->radius = 10;
        dongo->imoveable = false;
        dongo->eatable = true;
        dongo->col = PURPLE;

        break;
    }
    case 2: { // Two Suns (French) and Two Planets (American)
        planetsAmn = 9;
        planets = malloc(sizeof(Planet) * planetsAmn);

        Planet* toad = &planets[0];
        toad->pos.x = 600;
        toad->pos.y = 400;
        toad->delta.x = 0;
        toad->delta.y = 0;
        toad->radius = 20;
        toad->imoveable = true;
        toad->eatable = false;
        toad->col = WHITE;

        Planet* badoum = &planets[1];
        badoum->pos.x = 200;
        badoum->pos.y = 400;
        badoum->delta.x = 0;
        badoum->delta.y = 0;
        badoum->radius = 20;
        badoum->imoveable = true;
        badoum->eatable = false;
        badoum->col = WHITE;

        Planet* tito = &planets[2];
        tito->pos.x = 400;
        tito->pos.y = 200;
        tito->delta.x = 0;
        tito->delta.y = 0;
        tito->radius = 10;
        tito->imoveable = false;
        tito->eatable = false;
        tito->col = RED;

        Planet* dongo = &planets[3];
        dongo->pos.x = 400;
        dongo->pos.y = 210;
        dongo->delta.x = 1;
        dongo->delta.y = 2;
        dongo->radius = 6;
        dongo->imoveable = false;
        dongo->eatable = false;
        dongo->col = YELLOW;

        Planet* noodle = &planets[4];
        noodle->pos.x = 445;
        noodle->pos.y = 200;
        noodle->delta.x = 1;
        noodle->delta.y = 2;
        noodle->radius = 8;
        noodle->imoveable = false;
        noodle->eatable = false;
        noodle->col = PURPLE;

        Planet* schmicheal = &planets[5];
        schmicheal->pos.x = 300;
        schmicheal->pos.y = 400;
        schmicheal->delta.x = 2;
        schmicheal->delta.y = 0;
        schmicheal->radius = 6;
        schmicheal->imoveable = false;
        schmicheal->eatable = false;
        schmicheal->col = SKYBLUE;

        Planet* flare = &planets[6];
        flare->pos.x = 700;
        flare->pos.y = 700;
        flare->delta.x = 0;
        flare->delta.y = 0;
        flare->radius = 7;
        flare->imoveable = false;
        flare->eatable = false;
        flare->col = ORANGE;

        Planet* kel = &planets[7];
        kel->pos.x = 450;
        kel->pos.y = 450;
        kel->delta.x = 0;
        kel->delta.y = 0;
        kel->radius = 8;
        kel->imoveable = false;
        kel->eatable = false;
        kel->col = GREEN;

        Planet* starch = &planets[8];
        starch->pos.x = 900;
        starch->pos.y = 900;
        starch->delta.x = 0;
        starch->delta.y = 0;
        starch->radius = 12;
        starch->imoveable = false;
        starch->eatable = false;
        starch->col = PINK;

        break;
    }
    }

    int linePointAmn = 500;
    int totalLines = planetsAmn * linePointAmn;
    Vector2* lines = malloc(sizeof(Vector2) * totalLines);
    // Initialize points in line to invalid values
    for (int i = 0; i < totalLines; i++) lines[i] = (Vector2){ -1, -1 };

    InitWindow(screenWidth, screenHeight, "PlanetSim");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < planetsAmn; i++) {
            process(i, planets, planetsAmn, lines, linePointAmn);

            DrawCircle(planets[i].pos.x, planets[i].pos.y, planets[i].radius, planets[i].col);

            // Draw Planet Trail
            for (int j = 0; j < linePointAmn - 1; j++) {
                if (lines[j + i * linePointAmn].x == -1 && lines[j + i * linePointAmn].y == -1) continue;

                Vector2 point = lines[j + i * linePointAmn];
                Vector2 nextPoint = lines[j + 1 + i * linePointAmn];

                DrawLineEx(
                    lines[j + i * linePointAmn],
                    lines[j + 1 + i * linePointAmn],
                    planets[i].radius * 0.5,
                    (Color) { planets[i].col.r, planets[i].col.g, planets[i].col.b, 255 * ((float)j / linePointAmn) }
                );
            }
        }

        EndDrawing();
    }

    // Cleanup
    free(planets);
    free(lines);
    CloseWindow();
    return 0;
}