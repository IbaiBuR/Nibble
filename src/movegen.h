#pragma once

#include "types.h"

void initializeMoveList(MoveList *moveList);
void generateAllQuiets(const Board *board, MoveList *moveList, Color color);