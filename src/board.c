#include <stdio.h>
#include <string.h>
#include "board.h"

void print_board(board board)
{
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            printf("%d ", board_get(board, x, y));
        }

        printf("\n");
    }
}

int validate_hori(board board, size_t y)
{
    int donelist[9];
    for (size_t i = 0; i < 9; i++) donelist[i] = 0;
    
    for (size_t x = 0; x < 9; x++) {
        int val = board_get(board, x, y);

        if (val--) {
            if (donelist[val]) return 0;
            donelist[val] = 1;
        }
    }

    return 1;
}

int validate_vert(board board, size_t x)
{
    int donelist[9];
    for (size_t i = 0; i < 9; i++) donelist[i] = 0;

    for (size_t y = 0; y < 9; y++) {
        int val = board_get(board, x, y);

        if (val--) {
            if (donelist[val]) return 0;
            donelist[val] = 1;
        }
    }

    return 1;
}

int validate_box(board board, size_t x_, size_t y_)
{
    int donelist[9];
    for (size_t i = 0; i < 9; i++) donelist[i] = 0;
    
    size_t bx = x_ * 3;
    size_t by = y_ * 3;

    for (size_t x = bx; x < bx + 3; x++) {
        for (size_t y = by; y < by + 3; y++) {
            int val = board_get(board, x, y);

            if (val--) {
                if (donelist[val]) return 0;
                donelist[val] = 1;
            }
        }
    }

    return 1;
}

int validate_board(board board)
{
    for (size_t x = 0; x < 9; x++) {
        if (!validate_vert(board, x)) return 0;
    }

    for (size_t y = 0; y < 9; y++) {
        if (!validate_hori(board, y)) return 0;
    }

    for (size_t x = 0; x < 3; x++) {
        for (size_t y = 0; y < 3; y++) {
            if (!validate_box(board, x, y)) return 0;
        }
    }

    return 1;
}

int board_get(board board, size_t x, size_t y)
{
    return board[y * 9 + x];
}

void itoxy(size_t i, size_t *x, size_t *y)
{
    *x = i % 9;
    *y = (i - *x) / 9;
}

void neighbors(size_t x_, size_t y_, size_t *out)
{
    size_t out_i = 0;
    
    // box
    size_t bx = (x_ / 3) * 3;
    size_t by = (y_ / 3) * 3;

    for (size_t x = bx; x < bx + 3; x++) {
        for (size_t y = by; y < by + 3; y++) {
            if (x == x_ && y == y_) continue;

            out[out_i++] = x;
            out[out_i++] = y;
        }
    }

    // horizontal
    for (size_t x = 0; x < 9; x++) {
        if (x == x_ || (x >= bx && x < bx + 3)) continue;

        out[out_i++] = x;
        out[out_i++] = y_;
    }

    // vertical
    for (size_t y = 0; y < 9; y++) {
        if (y == y_ || (y >= by && y < by + 3)) continue;

        out[out_i++] = x_;
        out[out_i++] = y;
    }
}

void possi(board board, size_t x, size_t y, int *out, size_t *out_len)
{
    size_t neighbors_[40];
    neighbors(x, y, neighbors_);

    int exists[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (size_t i = 0; i <= 38; i += 2) {
        int g = board_get(board, neighbors_[i], neighbors_[i + 1]);
        if (g == 0) continue;
        exists[g - 1] = 1;
    }

    *out_len = 0;

    size_t i = 0;
    size_t out_i = 0;

    for (; i < 9; i++) {
        if (!exists[i]) {
            (*out_len)++;
            out[out_i++] = i + 1;
        }
    }
}

static void solve_(board board_, board *solutions, size_t *sol_len)
{
    // copy board
    board board_c;
    memcpy(board_c, board_, 81 * sizeof(int));

    int found_empty = 0;

    for (size_t i = 0; i < 81; i++) {
        size_t x, y;
        itoxy(i, &x, &y);
        
        if (board_c[i] == 0) {
            found_empty = 1;
            
            int possi_[9]; size_t possi_len;
            possi(board_c, x, y, possi_, &possi_len);

            if (possi_len == 0) return;

            for (size_t possi_i = 0; possi_i < possi_len; possi_i++) {
                board_c[i] = possi_[possi_i];
                solve_(board_c, solutions, sol_len);
            }

            break;
        }
    }

    if (!found_empty && *sol_len < 100) {
        // add to array of solution
        memcpy(solutions + *sol_len, board_c, sizeof(board));
        (*sol_len)++;
    }
}

void solve(board board_, board *solutions, size_t *sol_len)
{
    *sol_len = 0;
    solve_(board_, solutions, sol_len);
}
