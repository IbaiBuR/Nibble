#include "movegen.h"

#include "bitboard.h"

INLINE void addMove(const Move move, MoveList *moveList) {
    moveList->moves[moveList->count].move = move;
    moveList->moves[moveList->count].score = 0;
    moveList->count++;
}

static inline Bitboard
singlePawnPush(const Bitboard pawns, const Bitboard empty, const Color c) {
    return (c == WHITE) ? shiftN(pawns) & empty : shiftS(pawns) & empty;
}

static inline Bitboard
doublePawnPush(const Bitboard pawns, const Bitboard empty, const Color c) {
    return (c == WHITE)
             ? shiftN(singlePawnPush(pawns, empty, WHITE)) & empty & rank4
             : shiftS(singlePawnPush(pawns, empty, BLACK)) & empty & rank5;
}

void generateAllMoves(MoveList *moveList, Board *board) {}