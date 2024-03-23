#pragma once

#include "types.h"

void initializeMoveList(MoveList *moveList);
void printMoveList(MoveList moveList);

void generateAllQuiets(const Board *board, MoveList *moveList, Color color);
void generateAllCaptures(const Board *board, MoveList *moveList, Color color);
void generateAllMoves(const Board *board, MoveList *moveList, Color color);