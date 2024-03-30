#pragma once

#include <inttypes.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

uint32_t randU32();
uint64_t randU64();

long getTimeMS();
