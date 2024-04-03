#pragma once

#include "types.h"

void parsePosition(const char *command, Board *board);
void parseGo(const char *command, Board *board);
void uciLoop();