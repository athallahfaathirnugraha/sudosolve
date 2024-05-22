#include <raylib.h>
#include <stdlib.h>

int main()
{
    InitWindow(600, 600, "sudoku solver");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        EndDrawing();
    }

    CloseWindow();
}