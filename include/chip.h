#pragma once
#include "../include/display.h"
#include "stack.h"
#include "mem.h"

typedef struct chip_state {
    display* disp;
    unsigned short pc; // Program counter
    unsigned short ir; // Index register
    unsigned char delayTimer; // 8 bit timer
    unsigned char soundTimer; // 8 bit sound timer
    unsigned char registers[16]; // V0 - VF

    stack* s; // Has methods for pushing and popping from the stack

    unsigned char memory[4096]; // memory is 4096 bytes
} chip_state;
