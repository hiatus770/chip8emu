#pragma once

#include "globals.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct stack{
    unsigned short* arr;
    int len;
    int index;
} stack;

struct stack* init_stack(){
    stack* s = malloc(sizeof(stack));
    s->arr = (unsigned short*)malloc(2 * sizeof(short));
    s->len = 2;
    s->index = 0;
    return s;
}

void print_stack(stack* s){
    printf("Stack capacity: %d\n", s->len);
    printf("Stack index: %d\n", s->index);
    for (int i = 0; i < s->index; i++){
        printf("[%d] ", s->arr[i]);
    }
    printf("\n");
}

void push(stack* s, short data){
    if (s->index >= s->len){
        // Transfer over
        unsigned short* arr_new = (unsigned short*)malloc(s->len*2*sizeof(short));
        for (int i = 0; i < s->index; i++){
            arr_new[i] = s->arr[i];
        }
        // set new pointer
        free(s->arr;
        s->arr = arr_new;
    }

    // Set data
    s->arr[s->index] = data;
    s->index += 1; // Next spot
}

short pop(stack* s){
    if (s->index == 0){
        printf("ERROR: popping from empty stack");
        return 0;
    } else {
        short res = s->arr[s->index];
        s->index = s->index - 1;
        return res;
    }
}
