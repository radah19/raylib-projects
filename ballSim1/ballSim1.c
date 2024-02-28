#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;

    int radius;
    int id;
} Ball;

void newWall(Rectangle* walls, int index, float _x, float _y, float _width, float _height) {
    walls[index].x = _x;
    walls[index].y = _y;
    walls[index].width = _width;
    walls[index].height = _height;
}

int main(void) {
    const int screenDimensions = 960;

    int ballsAmn = 0;
    int ballsArrLength = 10;
    Ball* balls = malloc(sizeof(Ball) * ballsArrLength);

    int collidingPairsLen = 10;
    int colSize = 0;
    Ball** collidingPairs = malloc(sizeof(*collidingPairs) * 2 * collidingPairsLen);

    Rectangle* walls = malloc(sizeof(Rectangle) * 20);
    int wallsAmn = 1;
    Rectangle floor = { 0, screenDimensions - 32, screenDimensions, 32 };
    
    walls[0] = floor;

    InitWindow(screenDimensions, screenDimensions, "Ball Sim");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        //Player Input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (ballsAmn == ballsArrLength) {
                ballsArrLength *= 2;
                balls = realloc(balls, sizeof(Ball) * ballsArrLength);
            }

            Ball* b = &balls[ballsAmn];
            b->pos.x = GetMouseX();
            b->pos.y = GetMouseY();

            b->vel.x = 0;
            b->vel.y = 0;
            b->acc.x = 0;
            b->acc.y = 0.1f;

            b->radius = 12;
            b->id = ballsAmn;

            ballsAmn++;
        }

        //Physics Sim
        for (int i = 0; i < ballsAmn; i++) {
            Ball* b1 = &balls[i];

            b1->vel.x += b1->acc.x;
            b1->vel.y += b1->acc.y;

            b1->pos.x += b1->vel.x;
            b1->pos.y += b1->vel.y;

            // Basic Collisions
            for (int j = 0; j < ballsAmn; j++) {
                //Collision with ball
                Ball* b2 = &balls[j];

                if (b1->id != b2->id) {
                    if (CheckCollisionCircles(b1->pos, b1->radius, b2->pos, b2->radius)) {
                        // Add to Colliding Pairs
                        if (colSize == collidingPairsLen) {
                            collidingPairsLen *= 2;
                            collidingPairs = realloc(collidingPairs, sizeof(*collidingPairs) * 2 * collidingPairsLen);
                        }

                        collidingPairs[colSize] = b1;
                        collidingPairs[colSize+1] = b2;
                        colSize += 2;

                        //Distance between Ball Centers
                        float fDist = sqrtf((b1->pos.x - b2->pos.x) * (b1->pos.x - b2->pos.x)
                            + (b1->pos.y - b2->pos.y) * (b1->pos.y - b2->pos.y));
                        float fOverlap = (fDist - b1->radius - b2->radius) * 0.5f;

                        //Displacement
                        b1->pos.x -= fOverlap * (b1->pos.x - b2->pos.x) / fDist;
                        b1->pos.y -= fOverlap * (b1->pos.y - b2->pos.y) / fDist;
                        b2->pos.x += fOverlap * (b1->pos.x - b2->pos.x) / fDist;
                        b2->pos.y += fOverlap * (b1->pos.y - b2->pos.y) / fDist;
                    }
                }

                //Collision with walls
                for (int j = 0; j < wallsAmn; j++) {
                    if (CheckCollisionCircleRec(b1->pos, b1->radius, walls[j])) {
                        //Displacement
                        while (CheckCollisionCircleRec(b1->pos, b1->radius, walls[j]))
                            b1->pos.y -= 0.1;
                        b1->pos.y -= b1->vel.y;
                        b1->vel.y *= -0.3;
                    }
                }

            }            
        }

        // Dynamic Collisions
        for (int i = 0; i < colSize; i += 2) {
            Ball* b1 = collidingPairs[i];
            Ball* b2 = collidingPairs[i+1];
        }

        //Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < ballsAmn; i++) {
            DrawCircle(balls[i].pos.x, balls[i].pos.y, balls[i].radius, BLUE);
        }
        
        for (int i = 0; i < wallsAmn; i++) {
            DrawRectangle(walls[i].x, walls[i].y, walls[i].width, walls[i].height, RED);
        }
        
        for (int i = 0; i < colSize; i += 2) {
            DrawLineEx(collidingPairs[i]->pos, collidingPairs[i + 1]->pos, 1, GREEN);
        }

        EndDrawing();

        //Clear Colliding Pairs
        collidingPairs = malloc(sizeof(int) * 2 * collidingPairsLen);
        colSize = 0;
    }

    free(collidingPairs);
    free(balls);
    CloseWindow();
    return 0;
}