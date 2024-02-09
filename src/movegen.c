#include "movegen.h"

#include "bitboard.h"

inline Bitboard singlePawnPush(Bitboard pawns, Bitboard empty, Color c) {
    return (c == WHITE) ? shiftN(pawns) & empty : shiftS(pawns) & empty;
}

inline Bitboard doublePawnPush(Bitboard pawns, Bitboard emtpy, Color c) {
    return (c == WHITE)
             ? shiftN(singlePawnPush(pawns, emtpy, WHITE)) & emtpy & rank4
             : shiftS(singlePawnPush(pawns, emtpy, BLACK)) & emtpy & rank5;
}