#include "movegen.h"

#include "bitboard.h"

inline Bitboard singlePawnPush(Bitboard pawns, Bitboard empty, Color c) {
    return (c == WHITE) ? shiftN(pawns) & empty : shiftS(pawns) & empty;
}

inline Bitboard doublePawnPush(Bitboard pawns, Bitboard empty, Color c) {
    return (c == WHITE)
             ? shiftN(singlePawnPush(pawns, empty, WHITE)) & empty & rank4
             : shiftS(singlePawnPush(pawns, empty, BLACK)) & empty & rank5;
}