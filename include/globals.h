#pragma once
#include <stdio.h>

const int screenWidth = 1024;
const int screenHeight = 512;

// Configuration for the behaviour of the hardware
#define jumpOffsetNew 1
#define oldStoreLoad 0 
#define debug 0

// debugging helper
#if debug
#define debugf(...) printf(__VA_ARGS__)
#else
#define debugf(...) ((void)0)
#endif
