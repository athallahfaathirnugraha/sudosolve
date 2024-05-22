#include <raylib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"

#define BOARD_SIZE 549
#define CELL_SIZE BOARD_SIZE / 9

int main()
{
    board board;
    for (int i = 0; i < 81; i++) board[i] = -1;

    size_t s_cell[2] = { 0, 0 };
    
    InitWindow(BOARD_SIZE, BOARD_SIZE, "sudoku solver");

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_H) && s_cell[0] > 0) s_cell[0] -= 1;
        if (IsKeyPressed(KEY_J) && s_cell[1] < 8) s_cell[1] += 1;
        if (IsKeyPressed(KEY_K) && s_cell[1] > 0) s_cell[1] -= 1;
        if (IsKeyPressed(KEY_L) && s_cell[0] < 8) s_cell[0] += 1;

        {
            size_t i = s_cell[1] * 9 + s_cell[0];

            if (IsKeyPressed(KEY_I) && board[i] < 9) board[s_cell[1] * 9 + s_cell[0]] += 1;
            if (IsKeyPressed(KEY_O) && board[i] > -1) board[s_cell[1] * 9 + s_cell[0]] -= 1;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x <= BOARD_SIZE; x += CELL_SIZE) DrawLine(x, 0, x, BOARD_SIZE, BLACK);
        for (int y = 0; y <= BOARD_SIZE; y += CELL_SIZE) DrawLine(0, y, BOARD_SIZE, y, BLACK);

        for (int i = 0; i < 81; i++) {
            int num = board[i];

            if (num == -1) continue;

            char chr[2];
            sprintf(chr, "%d", num);

            int x = i % 9;
            int y = (i - x) / 9;

            int sx = x * CELL_SIZE;
            int sy = y * CELL_SIZE;

            DrawText(chr, sx, sy, 50, BLACK);
        }

        // draw selected cell
        DrawRectangleLines(
            s_cell[0] * CELL_SIZE,
            s_cell[1] * CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            BLUE
        );
        
        EndDrawing();
    }

    CloseWindow();
}