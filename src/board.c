#include "board.h"

int board_get(board board, size_t x, size_t y)
{
    return board[y * 9 + x];
}

void neighbors(size_t x_, size_t y_, size_t *out)
{
    size_t out_i = 0;
    
    // box
    size_t bx = (x_ / 3) * 3;
    size_t by = (y_ / 3) * 3;

    for (size_t x = bx; x < bx + 3; x++) {
        for (size_t y = by; y < bx + 3; y++) {
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