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
    stack* s = (stack*)malloc(sizeof(stack));
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
    if (s->index == 0){
        printf("[EMPTY STACK]");
    }
    printf("\n");
}

void push(stack* s, short data){
    if (s->index >= s->len){
        // Transfer over
        debugf("Resizing stack %d to %d as index is %d\n", s->len, s->len*2, s->index);
        unsigned short* arr_new = (unsigned short*)malloc(s->len*2*sizeof(short));
        for (int i = 0; i < s->index; i++){
            arr_new[i] = s->arr[i];
        }
        // set new pointer
        free(s->arr);
        s->arr = arr_new;
    }

    // Set data
    s->arr[s->index] = data;
    s->index += 1; // Next spot
}

unsigned short pop(stack* s){
    if (s->index == 0){
        printf("ERROR: popping from empty stack");
        return 0;
    } else {
        unsigned short res = s->arr[s->index-1];
        s->index = s->index - 1;
        return res;
    }
}
