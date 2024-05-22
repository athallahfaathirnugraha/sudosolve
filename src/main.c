#include <raylib.h>
#include <stdlib.h>

#define BOARD_SIZE 549
#define CELL_SIZE BOARD_SIZE / 9

int main()
{
    size_t s_cell[2] = { 0, 0 };
    
    InitWindow(BOARD_SIZE, BOARD_SIZE, "sudoku solver");

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_H) && s_cell[0] > 0) s_cell[0] -= 1;
        if (IsKeyPressed(KEY_J) && s_cell[1] < 8) s_cell[1] += 1;
        if (IsKeyPressed(KEY_K) && s_cell[1] > 0) s_cell[1] -= 1;
        if (IsKeyPressed(KEY_L) && s_cell[0] < 8) s_cell[0] += 1;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x <= BOARD_SIZE; x += CELL_SIZE) DrawLine(x, 0, x, BOARD_SIZE, BLACK);
        for (int y = 0; y <= BOARD_SIZE; y += CELL_SIZE) DrawLine(0, y, BOARD_SIZE, y, BLACK);

        DrawRectangleLines(s_cell[0] * CELL_SIZE, s_cell[1] * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
        
        EndDrawing();
    }

    CloseWindow();
}