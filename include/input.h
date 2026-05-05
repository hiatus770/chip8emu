#pragma once
#include "globals.h"
#include "raylib.h"

/**
 * Assumes that the size of the array is 16 long
 */
void get_keypad(char arr[16]){
    int keys[] = {
        KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR,
        KEY_Q,   KEY_W,   KEY_E,     KEY_R,
        KEY_A,   KEY_S,   KEY_D,     KEY_F,
        KEY_Z,   KEY_X,   KEY_C,     KEY_V
    };

    int order[] = {
        1,   2,   3,   0xC,
        4,   5,   6,   0xD,
        7,   8,   9,   0xE,
        0xA, 0,   0xB, 0xF
    };

    for(int i = 0; i < 16; i++){
        if (IsKeyDown(keys[i])){
            arr[i] = 1;
        } else {
            arr[i] = 0;
        }
    }
}
