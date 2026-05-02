#include <stdio.h>
#include "../include/globals.h"
#include "../include/display.h"
#include "../include/raylib.h"

#include "../include/stack.h"

int main(void)
{
    display* disp = init_display();

    InitWindow(screenWidth, screenHeight, "chip8emu");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            render_raylib(disp);
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
