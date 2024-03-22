#include "movegen.h"

#include "bitboard.h"
#include "board.h"
#include "move.h"

INLINE void addMove(const Move move, MoveList *moveList) {
    moveList->moves[moveList->count].move  = move;
    moveList->moves[moveList->count].score = 0;
    moveList->count++;
}

void initializeMoveList(MoveList *moveList) {
    moveList->count        = 0;
    moveList->moves->move  = NOMOVE;
    moveList->moves->score = 0;
}

static inline void
quietPawnMoves(const Board *board, MoveList *moveList, const Color c, const Bitboard empty) {
    const Bitboard  pawns     = pieceBB(board, PAWN, c);
    const Direction movingDir = c == WHITE ? SOUTH : NORTH;

    Bitboard singlePush = singlePawnPush(pawns, empty, c);
    Bitboard doublePush = doublePawnPush(pawns, empty, c);

    while (singlePush)
    {
        const Square to = popLsb(&singlePush);
        addMove(makeMove(to - movingDir, to, QUIET), moveList);
    }

    while (doublePush)
    {
        const Square to = popLsb(&doublePush);
        addMove(makeMove(to - movingDir * 2, to, DOUBLEPUSH), moveList);
    }
}

static inline void quietPieceMoves(const Board    *board,
                                   MoveList       *moveList,
                                   const Color     c,
                                   const PieceType pt,
                                   const Bitboard  emtpy) {
    Bitboard pieces = pieceBB(board, pt, c);

    while (pieces)
    {
        const Square from = popLsb(&pieces);
        Bitboard     possiblePieceMoves =
            getAttacksByPieceType(pt, from, board->occupancies[COLOR_NB]) & emtpy;

        while (possiblePieceMoves)
        {
            addMove(makeMove(from, popLsb(&possiblePieceMoves), QUIET), moveList);
        }
    }
}

inline void generateAllQuiets(const Board *board, MoveList *moveList, const Color c) {
    const Bitboard empty = ~board->occupancies[COLOR_NB];

    quietPawnMoves(board, moveList, c, empty);

    for (PieceType pt = KNIGHT; pt <= KING; pt++)
    {
        quietPieceMoves(board, moveList, c, pt, empty);
    }
}
