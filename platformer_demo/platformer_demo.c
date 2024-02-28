#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>

//Math Functions
float Signum(float num) {
    if (num == 0) return 0;
    return (num > 0) ? 1 : -1;
}

float Abs(float num) {
    return (num < 0) ? num * -1 : num;
}

//Collision Code
bool WithinXRange(Rectangle a, Rectangle b) {
    return  (a.x >= b.x + b.width && a.x <= b.x) ||
        (a.x + a.width >= b.x && a.x + a.width <= b.x + b.width);
}

bool WithinYRange(Rectangle a, Rectangle b) {
    return  (a.y >= b.y + b.height && a.y <= b.y) ||
        (a.y + a.height >= b.y && a.y + a.height <= b.y + b.height);
}

bool ApproachingRecX(Rectangle a, Rectangle b, float deltaA) {
    return a.x + deltaA + (a.width * (deltaA > 0)) >= b.x &&
        a.x + deltaA + (a.width * (deltaA > 0)) <= b.x + b.width;
}

bool ApproachingRecY(Rectangle a, Rectangle b, float deltaA) {
    return  a.y + deltaA + (a.height * (deltaA > 0)) >= b.y &&
        a.y + deltaA + (a.height * (deltaA > 0)) <= b.y + b.height;
}


int main(void) {
    int screenWidth = 1440;
    int screenHeight = 810;

    int cellSize = 32;

    InitWindow(screenWidth, screenHeight, "Platformer");
    SetTargetFPS(60);

    //Player Vars
    Rectangle player = { screenWidth / 2, screenHeight / 2, cellSize, cellSize };

    float playerSpdX = 0;
    float maxPlayerSpdX = 10;
    float playerSpdDeltaX = 0.15;
    bool collisionX = false;

    float playerSpdY = 0;
    float maxPlayerSpdY = 5;
    float grav = 0.2;
    bool collisionY = false;

    //Walls
    Rectangle* walls = malloc(sizeof(Rectangle) * 3);
    for (int i = 0; i < 3; i++) {
        walls[i].x = screenWidth / 2.0f;
        walls[i].y = screenHeight / 2.0f;
        walls[i].width = cellSize;
        walls[i].height = cellSize;
    }
    walls[0].y += cellSize * 4;
    walls[0].width = cellSize * 10;
    walls[1].x += cellSize * 6;
    walls[1].height = cellSize * 4;
    walls[2].x -= cellSize * 6;
    walls[2].height = cellSize * 4;



    while (!WindowShouldClose()) {

        playerSpdX = Lerp(playerSpdX, (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * maxPlayerSpdX, playerSpdDeltaX);
        playerSpdY = Lerp(playerSpdY, 5, grav);

        //Collisions
        for (int i = 0; i < 3; i++) {
            //X
            if (Abs(playerSpdX) > 0 && WithinYRange(player, walls[i])) {
                if (ApproachingRecX(player, walls[i], playerSpdX)) {

                    while (ceilf(player.x + (player.width * (playerSpdX > 0))) !=
                        walls[i].x + Signum(playerSpdX) + (walls[i].width * (playerSpdX < 0))) {
                        player.x += Signum(playerSpdX);
                    }
                    playerSpdX = 0;

                }
            }

            ////Y
            if (Abs(playerSpdY) > 0 && WithinXRange(player, walls[i])) {
                if (ApproachingRecY(player, walls[i], playerSpdY)) {

                    while (ceilf(player.y + (player.height * (playerSpdY > 0))) !=
                        walls[i].y + Signum(playerSpdY) + (walls[i].height * (playerSpdY < 0))) {
                        player.y += Signum(playerSpdY);
                    }
                    playerSpdY = 0;

                }
            }

            //Jump
            //if (IsKeyPressed(KEY_W) && playerSpdY == 0 && player.y + 1 <= walls[i].y && WithinXRange(player, walls[i])) {
            //    playerSpdY = -10;
            //}
            if (IsKeyPressed(KEY_W)) {
                playerSpdY = -20;
            }

        }


        //Movement
        player.x += playerSpdX;
        player.y += playerSpdY;

        //Lerp Fix
        if (Abs(playerSpdX) < 0.05) playerSpdX = 0;

        //Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(player.x, player.y, player.width, player.height, GREEN);

        for (int i = 0; i < 3; i++)
            DrawRectangle(walls[i].x, walls[i].y, walls[i].width, walls[i].height, GRAY);

        EndDrawing();
    }

    free(walls);
    CloseWindow();
    return 0;
}