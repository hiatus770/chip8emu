#pragma once

#include "globals.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
// Methods for rendering display


typedef struct display{
    int pixels[64];// first index is the x coordinate, 2nd index is the y coordinate 0 0
} display;

display* init_display(){
    return (display*)calloc(1, sizeof(display));
}

void paint(display* d, int x, int y, int on){
    if (x > 64 || x < 0 || y > 32 || y < 0){
        printf("ERROR: invalid coordinate supplied for paint");
        return;
    }

    if (on == 1){
        int intermediate = 1 << y;
        d->pixels[x] |= intermediate;
    } else {
        int intermediate = ~(1 << y);
        d->pixels[x] &= intermediate;
    }
}

char query_display(display* d, int x, int y){
    if (x > 64 || x < 0 || y > 32 || y < 0){
        printf("ERROR: invalid coordinate supplied for paint");
        return 0;
    }

    return (d->pixels[x] & (1 << y)) != 0;
}

void render_raylib(display* d){
    int blockSize = screenWidth/64;

    // Take the current status of the display and render it using the bit math
    for (int i = 0; i < 64; i++){
        for (int bit = 0; bit < 32; bit++){
            if (((d->pixels[i] >> bit) & 1) == 1){
                int posX = i * blockSize;
                int posY = bit * blockSize;
                DrawRectangle(posX, posY, blockSize, blockSize, WHITE);
            } else {
                int posX = i * blockSize;
                int posY = bit * blockSize;
                DrawRectangle(posX, posY, blockSize, blockSize, BLUE);
            }
        }
    }

}
