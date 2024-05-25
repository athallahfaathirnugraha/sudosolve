#include <raylib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"

#define BOARD_SIZE 549
#define CELL_SIZE BOARD_SIZE / 9

#ifdef RELEASE
#define LOG(x)
#else
#define LOG(...) printf(__VA_ARGS__)
#endif

int main()
{
    board board_;
    for (int i = 0; i < 81; i++) board_[i] = 0;

    size_t s_cell[2] = { 0, 0 };

    board solutions[100];
    size_t sol_len = 0;
    
    InitWindow(BOARD_SIZE, BOARD_SIZE, "sudoku solver");

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_H) && s_cell[0] > 0) s_cell[0] -= 1;
        if (IsKeyPressed(KEY_J) && s_cell[1] < 8) s_cell[1] += 1;
        if (IsKeyPressed(KEY_K) && s_cell[1] > 0) s_cell[1] -= 1;
        if (IsKeyPressed(KEY_L) && s_cell[0] < 8) s_cell[0] += 1;

        {
            size_t i = s_cell[1] * 9 + s_cell[0];

            if (IsKeyPressed(KEY_I) && board_[i] < 9) board_[i] += 1;
            if (IsKeyPressed(KEY_O) && board_[i] > 0) board_[i] -= 1;
        }

        if (IsKeyPressed(KEY_ENTER)) {
#ifndef RELEASE
            // neighbors
            {
                size_t neighbors_[40];
                neighbors(s_cell[0], s_cell[1], neighbors_);

                LOG("neighbors at %zu, %zu:\n", s_cell[0], s_cell[1]);

                size_t i1 = 1;
                size_t i2 = 0;

                while (i1 <= 20) {
                    LOG("%zu. x = %zu, y = %zu\n", i1, neighbors_[i2], neighbors_[i2 + 1]);

                    i2 += 2;
                    i1++;
                }
            }

            // possi
            {
                int possi_[9]; size_t possi_len;
                possi(board_, s_cell[0], s_cell[1], possi_, &possi_len);

                LOG("possi at %zu, %zu (%zu) total: ", s_cell[0], s_cell[1], possi_len);

                for (size_t i = 0; i < possi_len; i++)
                    LOG("%i, ", possi_[i]);

                LOG("\n");
            }
#endif

            solve(board_, solutions, &sol_len);

#ifndef RELEASE
            LOG("solutions (%zu):\n", sol_len);
            
            // iter solutions
            for (size_t i = 0; i < sol_len; i++) {
                print_board(solutions[i]);
            }
#endif
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x <= BOARD_SIZE; x += CELL_SIZE) DrawLine(x, 0, x, BOARD_SIZE, BLACK);
        for (int y = 0; y <= BOARD_SIZE; y += CELL_SIZE) DrawLine(0, y, BOARD_SIZE, y, BLACK);

        for (int i = 0; i < 81; i++) {
            int num = board_[i];

            if (num == 0) continue;

            char chr[2];
            sprintf(chr, "%d", num);

            size_t x, y;
            itoxy(i, &x, &y);

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