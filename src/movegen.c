#include "movegen.h"

#include "bitboard.h"

INLINE void addMove(Move move, MoveList *moveList) {
    moveList->moves[moveList->count] = move;
    moveList->count++;
}

static inline Bitboard singlePawnPush(Bitboard pawns, Bitboard empty, Color c) {
    return (c == WHITE) ? shiftN(pawns) & empty : shiftS(pawns) & empty;
}

static inline Bitboard doublePawnPush(Bitboard pawns, Bitboard empty, Color c) {
    return (c == WHITE)
             ? shiftN(singlePawnPush(pawns, empty, WHITE)) & empty & rank4
             : shiftS(singlePawnPush(pawns, empty, BLACK)) & empty & rank5;
}

void generateAllMoves(MoveList *moveList, Board *board) {}