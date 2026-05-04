/**
 * File is mainly helper functions for the memory and loading in the font and programs from a file
 */
#pragma once
#include "font.h"
#include "globals.h"

/**
 * Takes the offset that you want the font loaded into
 */
void load_font(unsigned char* mem){
    int offset = 80;
    // The default offset is from 050 to 9f which is from 80 to 159 which is 8 which checks out, since we are including 80
    for (int i = offset; i < offset + 80; i++){
        mem[i] = font[i - offset];
    }
}

/**
 * Load a program, assumes mem is sized 4096
 */
void load_instructions(int length, unsigned short instructions[], unsigned char* mem){
    int memind = 0;
    for (int i = 0; i < length; i++){
        debugf("Loading instruction %x", instructions[i]);
        debugf(" into bytes %d-%d\n", memind, memind+1);
        mem[memind+1] = instructions[i] & 0x00FF;
        mem[memind] = (instructions[i] & 0xFF00) >> 8;
        memind += 2;
    }
}

/**
 * Loads from file and the first byte is 200
 */
void load_from_path(char* path, unsigned char* mem){
    // Take path and load into mem the raw bytes
    int byte;
    FILE *fptr = fopen(path, "rb"); // Read from the file

    int ind = 0x200;
    while ((byte = fgetc(fptr)) != EOF){
        mem[ind++] = byte;
    }
    printf("Loaded %d bytes from ROM\n", ind - 0x200);

    fclose(fptr);
    return;
}
