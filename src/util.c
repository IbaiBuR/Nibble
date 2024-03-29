#include "util.h"

static uint32_t seed = 1804289383;

// XORShift algorithm
uint32_t randU32() {
    uint32_t num = seed;

    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;

    seed = num;

    return num;
}

uint64_t randU64() {

    const uint64_t n1 = (uint64_t)(randU32()) & 0xFFFF;
    const uint64_t n2 = (uint64_t)(randU32()) & 0xFFFF;
    const uint64_t n3 = (uint64_t)(randU32()) & 0xFFFF;
    const uint64_t n4 = (uint64_t)(randU32()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}
