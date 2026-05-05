#include <stdio.h>
#include "../include/globals.h"
#include "../include/display.h"
#include "../include/raylib.h"

#include "../include/stack.h"
#include "../include/chip.h"
#include "../include/input.h"

int main(void)
{
    chip_state* chip = init_chip_empty();
    chip->pc = 0x200;
    load_font(chip->memory);

    // load_from_path("roms/Particle Demo [zeroZshadow, 2008].ch8", chip->memory);
    // load_from_path("roms/3dviprmaze-schip.ch8", chip->memory);
    load_from_path("roms/slipperyslope.ch8", chip->memory);
    // load_from_path("roms/6-keypad.ch8", chip->memory);

    InitWindow(screenWidth, screenHeight, "chip8emu");

    SetTargetFPS(120);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (chip->delayTimer > 0){
            chip->delayTimer -= 1;
        }
        if (chip->soundTimer > 0){
            chip->soundTimer -= 1;
        }

        get_keypad(chip->keypad);
        for(int i = 0; i < 80; i++){
            cycle(chip);
            render_raylib(chip->disp);
        }

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
