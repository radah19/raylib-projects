#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
} Vector2i;

enum directions { Left, Right, Up, Down, None };

void shiftArray(Vector2i* arr, int curSize) {
    if (curSize == 1) return;

    for (int i = curSize - 1; i > 0; i -= 1) {
        arr[i].x = arr[i - 1].x;
        arr[i].y = arr[i - 1].y;
    }
}

int main(void) {
    const int screenDimensions = 640;
    const int cellSize = 32;

    InitWindow(screenDimensions, screenDimensions, "Snake");
    SetTargetFPS(60);

    int snakeSize = 1;
    Vector2i* snake = malloc(sizeof(Vector2i));

    //X,Y
    snake[0].x = screenDimensions / 2;
    snake[0].y = screenDimensions / 2;

    enum directions direction = None;

    const int delayTime = 3;
    int movementTimer = delayTime;
    int canMoveAgain = -1;

    bool gameOver = false;

    Vector2i apple = {
        (rand() % (screenDimensions - cellSize)) / 32 * 32,
        (rand() % (screenDimensions - cellSize)) / 32 * 32
    };

    while (!WindowShouldClose()) {

        //Keyboard Input
        if (canMoveAgain <= 0 && !gameOver) {
            if (IsKeyPressed(KEY_W) && direction != Down) { direction = Up; canMoveAgain = 3; }
            if (IsKeyPressed(KEY_A) && direction != Right) { direction = Left; canMoveAgain = 3; }
            if (IsKeyPressed(KEY_S) && direction != Up) { direction = Down; canMoveAgain = 3; }
            if (IsKeyPressed(KEY_D) && direction != Left) { direction = Right; canMoveAgain = 3; }
        }
        else canMoveAgain--;

        //Direction Management
        if (direction != None) {
            if (movementTimer > 0)
                movementTimer--;
            else {

                shiftArray(snake, snakeSize);
                movementTimer = delayTime;

                switch (direction) {
                case Left:
                    snake[0].x -= cellSize;
                    break;

                case Right:
                    snake[0].x += cellSize;
                    break;

                case Up:
                    snake[0].y -= cellSize;
                    break;

                case Down:
                    snake[0].y += cellSize;
                    break;
                }

            }
        }

        //On collecting an apple
        if (snake[0].x == apple.x && snake[0].y == apple.y) {
            snakeSize++;

            snake = realloc(snake, snakeSize * sizeof(Vector2i));

            snake[snakeSize - 1].x = apple.x;
            snake[snakeSize - 1].y = apple.y;

            apple.x = ((rand() % (screenDimensions - cellSize)) / 32 * 32);
            apple.y = ((rand() % (screenDimensions - cellSize)) / 32 * 32);
            
            printf("yum yum");
        }

        //Game Over Conditions

        for (int i = 1; i < snakeSize; i++) {
            switch (direction) {
                case Right: case Left:
                    if ( (snake[0].x + ((direction == Right) * cellSize) - ((direction == Left) * cellSize) == snake[i].x) &&
                          snake[0].y == snake[i].y ) {
                        gameOver = true; direction = None; printf("SHIT");
                    }
                    break;

                case Up: case Down:
                    if ( (snake[0].y + ((direction == Down) * cellSize) - ((direction == Up) * cellSize) == snake[i].y) &&
                          snake[0].x == snake[i].x ){
                        gameOver = true; direction = None; printf("FUCK");
                    }
                    break;
            }
        }

        if (snake[0].x < 0 || snake[0].x > screenDimensions || snake[0].y < 0 || snake[0].y > screenDimensions) {
            gameOver = true; direction = None;
        }

        //Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < snakeSize; i++) {
            DrawRectangle(snake[i].x, snake[i].y, cellSize - 4, cellSize - 4, BLUE);
        }
        DrawRectangle(apple.x, apple.y, cellSize - 4, cellSize - 4, RED);

        EndDrawing();
    }

    free(snake);
    CloseWindow();
    return 0;
}
