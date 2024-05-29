#include <raylib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "timer.h"

#define BOARD_SIZE 549
#define CELL_SIZE BOARD_SIZE / 9

#ifdef RELEASE
#define LOG(x)
#else
#define LOG(...) printf(__VA_ARGS__)
#endif

typedef enum state
{
    BOARD_STATE,
    SOLUTION_STATE,
} state;

int main()
{
    state state = BOARD_STATE;
    size_t solution_index = 0;
    
    board board_;
    for (int i = 0; i < 81; i++) board_[i] = 0;

    size_t s_cell[2] = { 0, 0 };

    board solutions[100];
    size_t sol_len = 0;

    timer invalid_timer = new_timer(1);
    finish_timer(&invalid_timer);
    
    InitWindow(BOARD_SIZE, BOARD_SIZE + 11, "sudoku solver");

    while (!WindowShouldClose()) {
        switch (state) {
            case BOARD_STATE:
                // selection
                if (IsKeyPressed(KEY_H) && s_cell[0] > 0) s_cell[0] -= 1;
                if (IsKeyPressed(KEY_J) && s_cell[1] < 8) s_cell[1] += 1;
                if (IsKeyPressed(KEY_K) && s_cell[1] > 0) s_cell[1] -= 1;
                if (IsKeyPressed(KEY_L) && s_cell[0] < 8) s_cell[0] += 1;

                // cells
                {
                    size_t i = s_cell[1] * 9 + s_cell[0];

                    if (IsKeyPressed(KEY_I) && board_[i] < 9) board_[i] += 1;
                    if (IsKeyPressed(KEY_O) && board_[i] > 0) board_[i] -= 1;

                    if (IsKeyPressed(KEY_ZERO)) board_[i] = 0;
                    if (IsKeyPressed(KEY_ONE)) board_[i] = 1;
                    if (IsKeyPressed(KEY_TWO)) board_[i] = 2;
                    if (IsKeyPressed(KEY_THREE)) board_[i] = 3;
                    if (IsKeyPressed(KEY_FOUR)) board_[i] = 4;
                    if (IsKeyPressed(KEY_FIVE)) board_[i] = 5;
                    if (IsKeyPressed(KEY_SIX)) board_[i] = 6;
                    if (IsKeyPressed(KEY_SEVEN)) board_[i] = 7;
                    if (IsKeyPressed(KEY_EIGHT)) board_[i] = 8;
                    if (IsKeyPressed(KEY_NINE)) board_[i] = 9;
                }

                // clear board
                if (IsKeyPressed(KEY_C)) {
                    for (int i = 0; i < 81; i++) board_[i] = 0;
                }

                // solve
                if (IsKeyPressed(KEY_ENTER)) {
                    // validate
                    if (validate_board(board_)) LOG("valid\n");
                    else {
                        LOG("invalid\n");
                        reset_timer(&invalid_timer);
                        goto skip_solve;
                    }

                    solve(board_, solutions, &sol_len);
                    state = SOLUTION_STATE;
                    solution_index = 0;

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

                    LOG("solutions (%zu):\n", sol_len);
            
                    // iter solutions
                    for (size_t i = 0; i < sol_len; i++) {
                        print_board(solutions[i]);
                        LOG("\n");
                    }
#endif
                }

skip_solve:

                break;
            case SOLUTION_STATE:
                if (IsKeyPressed(KEY_L) && solution_index < sol_len - 1) solution_index++;
                if (IsKeyPressed(KEY_H) && solution_index > 0) solution_index--;

                if (IsKeyPressed(KEY_B)) {
                    state = BOARD_STATE;
                }

                break;
        }

        board *drawn_board;

        switch (state) {
            case BOARD_STATE:
                drawn_board = &board_;
                break;
            case SOLUTION_STATE:
                drawn_board = &solutions[solution_index];
                break;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x <= BOARD_SIZE; x += CELL_SIZE) DrawLine(x, 0, x, BOARD_SIZE, BLACK);
        for (int y = 0; y <= BOARD_SIZE; y += CELL_SIZE) DrawLine(0, y, BOARD_SIZE, y, BLACK);

        for (int i = 0; i < 81; i++) {
            int num = (*drawn_board)[i];

            if (num == 0) continue;

            char chr[2];
            sprintf(chr, "%d", num);

            size_t x, y;
            itoxy(i, &x, &y);

            int sx = x * CELL_SIZE;
            int sy = y * CELL_SIZE;

            DrawText(chr, sx, sy, 50, BLACK);
        }

        // draw selected cell if board state
        if (state == BOARD_STATE) {
            DrawRectangleLines(
                s_cell[0] * CELL_SIZE,
                s_cell[1] * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                BLUE
            );
        }

        // bottom text
        char btext[1024];

        if (!update_timer(&invalid_timer)) {
            strcpy(btext, "nulisnya yg bener atuh");
        } else {
            switch (state) {
                case BOARD_STATE:
                    strcpy(btext, "athallahfaathirnugraha");
                    break;
                case SOLUTION_STATE:
                    strcpy(btext, TextFormat("found %zu/%zu solutions", solution_index + 1, sol_len));
                    break;
            }
        }

        DrawText(btext, 10, BOARD_SIZE + 1, 10, BLACK);
        
        EndDrawing();
    }

    CloseWindow();
}