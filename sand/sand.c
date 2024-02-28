#include "raylib.h"

#include <stdlib.h>
#include <stdio.h>

int main(void) {

    int dimensions = 750;
    int cellSize = 10;
    int boardLen = dimensions / cellSize;
    int boardSize = boardLen * boardLen;

    InitWindow(dimensions, dimensions, "Sand");
    SetTargetFPS(60);

    int* board = malloc(sizeof(int) * boardSize);
    
    for(int i = 0; i < boardSize; i++) board[i] = 0;

    while (!WindowShouldClose()) {
        
        //Creation Events
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            board[ ((GetMouseX()/cellSize)%boardLen) + ((GetMouseY()/cellSize)*boardLen) ] = 1;     
        
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            board[ ((GetMouseX()/cellSize)%boardLen) + ((GetMouseY()/cellSize)*boardLen) ] = 2;  

        //Board Events
        for (int i = boardSize - 1; i >= 0; i--) {
            switch (board[i]) {
            
            case 1: //Sand         
                if (i < boardSize - boardLen) {
                    if(board[i + boardLen] != 1){
                        board[i + boardLen] = 1;
                        board[i] = 0;
                    }
                    else if(i%boardLen > 0 && board[i + boardLen - 1] != 1){
                        board[i + boardLen - 1] = 1;
                        board[i] = 0;
                    }
                    else if(i%boardLen < boardLen-1 && board[i + boardLen + 1] != 1){
                        board[i + boardLen + 1] = 1;
                        board[i] = 0;
                    }
                }
                
                break;
                
            case 2: //Water
                if (i < boardSize - boardLen) {
                    if(board[i + boardLen] == 0){
                        board[i + boardLen] = 2;
                        board[i] = 0;
                    }
                    else if(i%boardLen > 0 && board[i - 1] == 0){
                        board[i - 1] = 2;
                        board[i] = 0;
                    }
                    else if(i%boardLen < boardLen-1 && board[i + 1] == 0){
                        board[i + 1] = 2;
                        board[i] = 0;
                    }
                }


                break;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        for (int i = 0; i < boardSize; i++) {
            switch(board[i]){
                case 1:
                    DrawRectangle(i%boardLen * cellSize, i/boardLen * cellSize, cellSize, cellSize, YELLOW);
                    break;
                
                case 2:
                    DrawRectangle(i%boardLen * cellSize, i/boardLen * cellSize, cellSize, cellSize, DARKBLUE);
                    break;
            }
        }

        EndDrawing();
    }

    free(board);
    CloseWindow();
    return 0;
}