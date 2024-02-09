#pragma once

#include "types.h"

Bitboard singlePawnPush(Bitboard bitboard, Bitboard empty, Color color);
Bitboard doublePawnPush(Bitboard bitboard, Bitboard empty, Color color);

void generateMoves(MoveList *moveList, Board *board);

