#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdlib.h>

// 0 = empty cell
typedef int board[81];

int board_get(board board, size_t x, size_t y);
void itoxy(size_t i, size_t *x, size_t *y);

// usage:
// size_t neighbors_[40];
// neighbors(x, y, neighbors_);
// 1st neighbor is x = neighbors[0], y = neighbors[1]
// 2nd neighbor is x = neighbors[2], y = neighbors[4]
void neighbors(size_t x, size_t y, size_t *out);

// usage:
// int possi_[9]; size_t possi_len;
// possi(board, x, y, possi_, &possi_len);
void possi(board board, size_t x, size_t y, int *out, size_t *out_len);

// you must free the solutions later.
void solve(board board_, board *solutions, size_t *sol_len);

#endif