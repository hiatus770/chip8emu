#include <stdio.h>
#include "../include/globals.h"
#include "../include/display.h"
#include "../include/raylib.h"

#include "../include/stack.h"
#include "../include/chip.h"

int main(void)
{
    chip_state* chip = init_chip_empty();
    chip->pc = 0x200;

    load_from_path("roms/3-corax+.ch8", chip->memory);

    InitWindow(screenWidth, screenHeight, "chip8emu");

    SetTargetFPS(30);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            for(int i = 0; i < 16; i++){
                cycle(chip);
            }
            render_raylib(chip->disp);
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
