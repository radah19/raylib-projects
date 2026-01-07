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
} Planet;

void process(int index, Planet* planets, int planetsAmn) {
    // Calculates where the provided planet should go every frame
    Planet* p = &planets[index];

    if (!p->imoveable) {
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
    }
}

void initPlanetsArr(Planet** planets, int* planetsAmn, int key) {
    

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

            Planet* toad = &planets[1];
            toad->pos.x = 400;
            toad->pos.y = 200;
            toad->delta.x = 3;
            toad->delta.y = 0;
            toad->radius = 10;
            toad->imoveable = false;
            toad->eatable = true;

            Planet* badoum = &planets[2];
            badoum->pos.x = 400;
            badoum->pos.y = 600;
            badoum->delta.x = -3;
            badoum->delta.y = 0;
            badoum->radius = 10;
            badoum->imoveable = false;
            badoum->eatable = true;

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

            Planet* toad = &planets[1];
            toad->pos.x = 400;
            toad->pos.y = 200;
            toad->delta.x = 3;
            toad->delta.y = 0;
            toad->radius = 10;
            toad->imoveable = false;
            toad->eatable = true;

            Planet* badoum = &planets[2];
            badoum->pos.x = 400;
            badoum->pos.y = 600;
            badoum->delta.x = -3;
            badoum->delta.y = 0;
            badoum->radius = 10;
            badoum->imoveable = false;
            badoum->eatable = true;

            Planet* tito = &planets[3];
            tito->pos.x = 200;
            tito->pos.y = 400;
            tito->delta.x = 0;
            tito->delta.y = -3;
            tito->radius = 10;
            tito->imoveable = false;
            tito->eatable = true;

            Planet* dongo = &planets[4];
            dongo->pos.x = 600;
            dongo->pos.y = 400;
            dongo->delta.x = 0;
            dongo->delta.y = 3;
            dongo->radius = 10;
            dongo->imoveable = false;
            dongo->eatable = true;

            break;
        }
        case 2: { // Two Suns (French) and Two Planets (American)
            planetsAmn = 4;
            planets = malloc(sizeof(Planet) * planetsAmn);

            Planet* toad = &planets[0];
            toad->pos.x = 600;
            toad->pos.y = 400;
            toad->delta.x = 0;
            toad->delta.y = 0;
            toad->radius = 20;
            toad->imoveable = true;
            toad->eatable = false;

            Planet* badoum = &planets[1];
            badoum->pos.x = 200;
            badoum->pos.y = 400;
            badoum->delta.x = 0;
            badoum->delta.y = 0;
            badoum->radius = 20;
            badoum->imoveable = true;
            badoum->eatable = false;

            Planet* tito = &planets[2];
            tito->pos.x = 400;
            tito->pos.y = 200;
            tito->delta.x = 0;
            tito->delta.y = 0;
            tito->radius = 10;
            tito->imoveable = false;
            tito->eatable = false;

            Planet* dongo = &planets[3];
            dongo->pos.x = 400;
            dongo->pos.y = 210;
            dongo->delta.x = 1;
            dongo->delta.y = 2;
            dongo->radius = 5;
            dongo->imoveable = false;
            dongo->eatable = false;

            break;
        }
    }

    InitWindow(screenWidth, screenHeight, "PlanetSim");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        
        for (int i = 0; i < planetsAmn; i++) {
            process(i, planets, planetsAmn);

            DrawCircle(planets[i].pos.x, planets[i].pos.y, planets[i].radius, WHITE);
        }

        EndDrawing();
    }

    // Cleanup
    free(planets);
    CloseWindow();
    return 0;
}