#pragma once
#include <stdio.h>

const int screenWidth = 1024;
const int screenHeight = 512;

// Configuration for the behaviour of the hardware
#define jumpOffsetNew 0
#define debug 1

#if debug
#define debugf(...) printf(__VA_ARGS__)
#else
#define debugf(...) ((void)0)
#endif
