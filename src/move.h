#pragma once

#include "types.h"

#define makeMove(from, to, flag) ((from) | ((to) << 6) | (flag))
#define from(move)               ((move & 0x3F))
#define to(move)                 (((move >> 6) & 0x3F))

char *moveToString(Move move);
