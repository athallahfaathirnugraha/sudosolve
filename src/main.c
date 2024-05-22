#include <raylib.h>
#include <stdlib.h>

#define WIDTH 594
#define HEIGHT 594

int main()
{
    InitWindow(WIDTH, HEIGHT, "sudoku solver");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x <= WIDTH; x += WIDTH / 9) DrawLine(x, 0, x, HEIGHT, BLACK);
        for (int y = 0; y <= HEIGHT; y += HEIGHT / 9) DrawLine(0, y, WIDTH, y, BLACK);
        
        EndDrawing();
    }

    CloseWindow();
}