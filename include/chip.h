#pragma once
#include "../include/display.h"
#include "globals.h"
#include "raylib.h"
#include "stack.h"
#include "mem.h"
#include <stdalign.h>

typedef struct chip_state {
    display* disp;
    unsigned short pc; // Program counter - 16 bit
    unsigned short ir; // Index register - 16 bit
    unsigned char delayTimer; // 8 bit timer
    unsigned char soundTimer; // 8 bit sound timer
    unsigned char registers[16]; // V0 - VF 8 bit registers

    stack* s; // Has methods for pushing and popping from the stack

    unsigned char memory[4096]; // memory is 4096 bytes
    //    3 2 1 0
    //    0 0 0 0


    char keypad[16]; // Keypad init

} chip_state;

/**
 * Initializes the chip to an empty state
 */
chip_state* init_chip_empty(){
    chip_state* chip = (chip_state*)malloc(sizeof(chip_state));

    chip->disp = init_display();
    chip->pc = 0;
    chip->ir = 0;
    chip->delayTimer = 0;
    chip->soundTimer = 0;

    for (int i = 0; i < 16; i++){
        chip->registers[i] = 0;
    }

    for (int i = 0; i < 4096; i++){
        chip->memory[i] = 0;
    }

    for (int i = 0; i < 16; i++){
        chip->keypad[i] = 0;
    }

    chip->s = init_stack();

    return chip;
}

/** Timer related opcodes */
void set_timer_inst(chip_state* chip, short reg){
    chip->registers[reg] = chip->delayTimer;
}
void delay_set_inst(chip_state* chip, short reg){
    chip->delayTimer = chip->registers[reg];
}
void sound_set_inst(chip_state* chip, short reg){
    chip->soundTimer = chip->registers[reg];
}

void dec_conv_inst(chip_state* chip, short reg){
    chip->memory[chip->ir] = chip->registers[reg]/100;
    chip->memory[chip->ir+1] = (chip->registers[reg] % 100)/10;
    chip->memory[chip->ir+2] = chip->registers[reg] % 10;
    debugf("Converting %d to %d %d %d\n", chip->registers[reg], chip->memory[chip->ir], chip->memory[chip->ir+1], chip->memory[chip->ir+2]);
}

/** Opcode 1nnn */
void jump_inst(chip_state* chip, unsigned int addr){
    debugf("Jumping to %x\n", addr);
    chip->pc = addr;
    return;
}

/**
 * Jump offset
 */
void jump_offset_inst(chip_state* chip, unsigned short n2, unsigned short n1, unsigned short n0){
    if (jumpOffsetNew){
        chip->pc = chip->registers[n2] + ((n2 << 8) + (n1 << 4) + n0);
    } else {
        chip->pc = chip->registers[0]  + ((n2 << 8) + (n1 << 4) + n0);
    }
}

/**
 * Input handling instructions
 */
void skip_press_inst(chip_state* chip, short reg){
    if (chip->keypad[chip->registers[reg]] == 1){
        chip->pc += 2;
    }
}
void skip_not_press_inst(chip_state* chip, short reg){
    if (chip->keypad[chip->registers[reg]] == 0){
        chip->pc += 2;
    }
}
void get_key_inst(chip_state* chip, short reg){
    debugf("Waiting on keypress %x\n" , chip->registers[reg]);
    if (chip->keypad[chip->registers[reg]] == 0){
        chip->pc -= 2; // keep it at the same spot
    }
}

/** Opcode 2NNN - call */
void call_inst(chip_state* chip, unsigned int addr){
    debugf("Calling %x saving pc %d\n", addr, chip->pc);
    print_stack(chip->s);
    push(chip->s, chip->pc);
    print_stack(chip->s);
    jump_inst(chip, addr);
    return;
}

/** Opcode 00EE - ret */
void ret_inst(chip_state* chip){
    debugf("CALLING RET\n");
    print_stack(chip->s);
    unsigned short res = pop(chip->s);
    print_stack(chip->s);
    debugf("Returning to %x\n", res);
    chip->pc = res;
}

/** Clear screen */
void clear_screen_inst(chip_state* chip){
    debugf("Clearing screen\n");
    for (int i = 0; i < 64; i++){
        chip->disp->pixels[i] = 0;
    }
}

/**
 * Set register 6XNN
 */
void set_register_inst(chip_state* chip, int reg_index, unsigned short value){
    debugf("Setting register %d to %x\n", reg_index, value);
    chip->registers[reg_index] = value;
}

/**
 * Load store instructions
 */
void store_reg_inst(chip_state* chip, short x){
    for (int i = 0; i <= x; i++){
        chip->memory[i + chip->ir] = chip->registers[i];
    }

    if (oldStoreLoad){
        chip->ir = chip->ir + x + 1;
    }
}
void load_reg_inst(chip_state* chip, short x){
    for (int i = 0; i <= x; i++){
        chip->registers[i] = chip->memory[i + chip->ir];
    }

    if (oldStoreLoad){
        chip->ir = chip->ir + x + 1;
    }
}

/**
 * add_register_inst 7XNN
 */
void add_register_inst(chip_state* chip, int reg_index, unsigned short value){
    debugf("Adding to register %d + %x\n", reg_index, value);
    chip->registers[reg_index] += value;
}

/**
 * Set index register ANNN
 */
void set_index_inst(chip_state* chip, unsigned int value){
    debugf("Setting ir to %x\n", value);
    chip->ir = value;
}
/**
 *  Add to index
 */
void add_to_index_inst(chip_state* chip, short reg){
    chip->ir += chip->registers[reg];
}
/**
 * Index font instructions, points to the part in memory with the font data
 */
void index_font_inst(chip_state* chip, short reg){
    debugf("Setting ir for char %x", chip->registers[reg]);
    chip->ir = chip->memory[80 + chip->registers[reg] * 5];
}

/**
 * Skip instruction 3xnn
 */
void skip_equal_inst(chip_state* chip, unsigned short x, unsigned short nn){
    debugf("Skip inst comparing %x == %x\n", chip->registers[x], nn);
    if (chip->registers[x] == nn){
        chip->pc += 2;
    }
}

/**
 * Skip instruction 4xnn not equal
 */
void skip_not_equal_inst(chip_state* chip, unsigned short x, unsigned short nn){
    debugf("Skip inst comparing %x != %x\n", chip->registers[x], nn);
    if (chip->registers[x] != nn){
        chip->pc += 2;
    }
}

/**
 * Skip reg equal
 */
void skip_reg_equal_inst(chip_state* chip, unsigned short xind, unsigned short yind){
    debugf("Skip inst comparing reg %x == %x", xind, yind);
    if (chip->registers[xind] == chip->registers[yind]){
        chip->pc += 2;
    }
}

/**
 * Skip reg not equal
 */
void skip_reg_not_equal_inst(chip_state* chip, unsigned short xind, unsigned short yind){
    debugf("Skip inst comparing reg %x == %x", xind, yind);
    if (chip->registers[xind] != chip->registers[yind]){
        chip->pc += 2;
    }
}



// Logical and arith instructoins
void log_arith_inst(chip_state* chip, unsigned short xind, unsigned short yind, char type){
    unsigned char xval = chip->registers[xind];
    unsigned char yval = chip->registers[yind];
    switch(type){
        case 0:
            chip->registers[xind] = yval;
            break;
        case 1:
            chip->registers[xind] = xval | yval;
            break;
        case 2:
            chip->registers[xind] = xval & yval;
            break;
        case 3:
            chip->registers[xind] = xval ^ yval;
            break;
        case 4:
            if ((unsigned int)xval + (unsigned int)yval > 255){
                chip->registers[0xF] = 1;
            } else {
                chip->registers[0xF] = 0;
            }
            chip->registers[xind] = xval + yval;
            break;
        case 5:
            if (xval >= yval){
                chip->registers[0xF] = 1;
            } else {
                chip->registers[0xF] = 0;
            }
            chip->registers[xind] = xval - yval;
            break;
        case 7:
            if (yval >= xval){
                chip->registers[0xF] = 1;
            } else {
                chip->registers[0xF] = 0;
            }
            chip->registers[xind] = yval - xval;
            break;
        case 6:
            if (yval & 0x1){
                chip->registers[0xF] = 1;
            } else {
                chip->registers[0xF] = 0;
            }
            chip->registers[xind] = yval;
            chip->registers[xind] >>= 1;
            break;
        case 0xE:
            if ((yval & 0x8000) != 0){
                chip->registers[0xF] = 1;
            } else {
                chip->registers[0xF] = 0;
            }
            chip->registers[xind] = yval;
            chip->registers[xind] <<= 1;
            break;
    }
}

void random_inst(chip_state* chip, unsigned char ind, unsigned short val){
    debugf("Generating random number for V%x", ind, " up to %d", val);
    chip->registers[ind] = rand() & val;
    return;
}

/**
 * Display instruction
 */
void display_inst(chip_state* chip, short x_ind, short y_ind, unsigned short n){
    short x_coord = chip->registers[x_ind] & 63;
    short y_coord = chip->registers[y_ind] & 63;
    debugf("Calling display on coords %d, %d with %d rows\n", x_coord, y_coord, n);
    debugf("IR is: %x\n", chip->ir);
    debugf("Memory at 0x200: %x %x\n", chip->memory[0x200], chip->memory[0x201]);

    chip->registers[0xF] = 0; // set vf to zero
    unsigned short starting_index = chip->ir;

    for (int i = 0; i < n; i++){
        char data = chip->memory[starting_index + i];
        debugf("Data to render: %x\n", data);
        if (y_coord + i > 31){
            continue;
        }
        for (int j = 0; j < 8; j++){
            if (x_coord + j > 63){
                continue;
            }
            if (query_display(chip->disp, x_coord + j, y_coord + i) && ((data >> (7-j)) & 1)){
                paint(chip->disp, x_coord + j, y_coord + i, 0);
                chip->registers[0xF] = 1; // Set vf to one since we removed pixel
            } else if (query_display(chip->disp, x_coord + j, y_coord + i) == 0 && ((data >> (7-j)) & 1) & 1){
                paint(chip->disp, x_coord + j, y_coord + i, 1);
            } else {
                debugf("nothing to render ");
            }
        }
        debugf("\n");
    }

}


void cycle(chip_state* chip){

    // First fetch instruction
    debugf("Fetching instruction at pc %d\n", chip->pc);
    unsigned short inst = (chip->memory[chip->pc] << 8) + chip->memory[chip->pc+1];
    chip->pc += 2; // Increment by two
    debugf("Instruction: %x\n", inst);

    // Decode instruction for the chip
    unsigned int n3 = (inst >> 12) & 0xF;
    unsigned int n2 = (inst >> 8) & 0xF;
    unsigned int n1 = (inst >> 4) & 0xF;
    unsigned int n0 = (inst) & 0xF;
    unsigned int n20 = (inst) & 0x0FFF;
    unsigned int n10 = (inst) & 0x00FF;

    debugf("%x %x %x %x\n", n3, n2, n1, n0);

    switch (n3){
        case 0x0:
            if (n20 == 0x0E0){
                // Clear screen
                clear_screen_inst(chip);
            }
            if (n20 == 0x0EE){
                // Return
                ret_inst(chip);
            }
            break;
        case 0x1:
            // Jump directly to the last 12 bits
            jump_inst(chip, n20);
            break;
        case 0x2:
            // Call instruction
            call_inst(chip, n20);
            break;
        case 0xB:
            //  Has 2 sets of behaviour configured in globals.h
            jump_offset_inst(chip, n2, n1, n0);
            break;
        // random
        case 0xC:
            // Gen reandom
            random_inst(chip, n2, n10);
            break;
        case 0x3:
            // Skip instructoin
            skip_equal_inst(chip, n2, n10);
            break;
        case 0x4:
            skip_not_equal_inst(chip, n2, n10);
            break;
        case 0x5:
            skip_reg_equal_inst(chip, n2, n1);
            break;
        case 0x9:
            skip_reg_not_equal_inst(chip, n2, n1);
            break;

        case 0x6:
            // This is the set register set of commands  6XNN
            //                                           3210
            set_register_inst(chip, n2, n10);
            break;
        case 0x7:
            add_register_inst(chip, n2, n10);
            break;
        case 0xA:
            // Set index
            set_index_inst(chip, n20);
            break;

        // Logical / arithmetic operations
        case 0x8:
            log_arith_inst(chip, n2, n1, n0);
            break;

        // Input
        case 0xE:
            // Both input skipping instructions
            switch(n10){
                case 0x9E:
                    skip_press_inst(chip, n2);
                    break;
                case 0xA1:
                    skip_not_press_inst(chip, n2);
                    break;
            }
            break;

        // F opcodes
        case 0xF:
            switch(n10){
                // Timer related opcodes
                case 0x07:
                    set_timer_inst(chip, n2);
                    break;
                case 0x15:
                    delay_set_inst(chip, n2);
                    break;
                case 0x18:
                    sound_set_inst(chip, n2);
                    break;

                // Add to index
                case 0x1E:
                    add_to_index_inst(chip, n2);
                    break;

                // Get key
                case 0x0A:
                    get_key_inst(chip, n2); // waits for instructions
                    break;

                // Get font
                case 0x29:
                    index_font_inst(chip, n2);
                    break;

                // Binary coded decimal conversion
                case 0x33:
                    dec_conv_inst(chip, n2);
                    break;

                // Store and load memory
                case 0x55:
                    store_reg_inst(chip, n2);
                    break;

                case 0x65:
                    load_reg_inst(chip, n2);
                    break;

            }
            break;

        // Miscellaneous
        case 0xD:
            // Display instruction
            display_inst(chip, n2, n1, n0); // x y n rows
            break;
        default:
            debugf("Instruction not found : %x", inst);
            break;
    }

}


// Extract parts of the opcodes
