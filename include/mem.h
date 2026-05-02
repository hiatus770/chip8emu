/**
 * File is mainly helper functions for the memory and loading in the font and programs from a file
 */
#pragma once
#include "font.h"

/**
 * Takes the offset that you want the font loaded into
 */
void load_font(unsigned char* mem, int offset = 80){
    // The default offset is from 050 to 9f which is from 80 to 159 which is 8 which checks out, since we are including 80
    for (int i = offset; i < offset + 80; i++){
        mem[i] = font[i - offset];
    }
}
