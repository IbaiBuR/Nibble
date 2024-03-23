#include "movegen.h"

#include <stdio.h>

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

void printMoveList(const MoveList moveList) {
    for (uint32_t i = 0; i < moveList.count; i++)
    {
        printf("%d. %-2s\n", i, moveToString(moveList.moves[i].move));
    }
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
        addMove(buildMove(to - movingDir, to, QUIET), moveList);
    }

    while (doublePush)
    {
        const Square to = popLsb(&doublePush);
        addMove(buildMove(to - movingDir * 2, to, DOUBLEPUSH), moveList);
    }
}

static inline void pawnCaptures(const Board *board, MoveList *moveList, const Color c) {
    Bitboard pawns = pieceBB(board, PAWN, c);

    while (pawns)
    {
        const Square from                 = popLsb(&pawns);
        Bitboard     possiblePawnCaptures = pawnAttacks[c][from] & board->occupancies[c ^ 1];

        while (possiblePawnCaptures)
        {
            addMove(buildMove(from, popLsb(&possiblePawnCaptures), CAPTURE), moveList);
        }
    }

    if (board->epSq != NO_SQ)
    {
        Bitboard epPawns = pawnAttacks[c ^ 1][board->epSq] & pieceBB(board, PAWN, c);

        while (epPawns)
        {
            addMove(buildMove(popLsb(&epPawns), board->epSq, ENPASSANT), moveList);
        }
    }
}

static inline void quietPieceMoves(const Board    *board,
                                   MoveList       *moveList,
                                   const Color     c,
                                   const PieceType pt,
                                   const Bitboard  empty) {
    Bitboard pieces = pieceBB(board, pt, c);

    while (pieces)
    {
        const Square from = popLsb(&pieces);
        Bitboard     possiblePieceMoves =
            getAttacksByPieceType(pt, from, board->occupancies[COLOR_NB]) & empty;

        while (possiblePieceMoves)
        {
            addMove(buildMove(from, popLsb(&possiblePieceMoves), QUIET), moveList);
        }
    }
}

static inline void
pieceCaptures(const Board *board, MoveList *moveList, const Color c, const PieceType pt) {
    Bitboard pieces = pieceBB(board, pt, c);

    while (pieces)
    {
        const Square from = popLsb(&pieces);
        Bitboard     possiblePieceCaptures =
            getAttacksByPieceType(pt, from, board->occupancies[COLOR_NB]) & board->occupancies[c ^ 1];

        while (possiblePieceCaptures)
        {
            addMove(buildMove(from, popLsb(&possiblePieceCaptures), CAPTURE), moveList);
        }
    }
}

inline void generateAllQuiets(const Board *board, MoveList *moveList, const Color c) {

    if (bitCount(board->checkers) < 2)
    {
        quietPawnMoves(board, moveList, c, ~board->occupancies[COLOR_NB]);

        for (PieceType pt = KNIGHT; pt <= KING; pt++)
        {
            quietPieceMoves(board, moveList, c, pt, ~board->occupancies[COLOR_NB]);
        }
    }
    else
    {
        quietPieceMoves(board, moveList, c, KING, ~board->occupancies[COLOR_NB]);
    }
}

inline void generateAllCaptures(const Board *board, MoveList *moveList, const Color c) {

    if(bitCount(board->checkers < 2))
    {
        pawnCaptures(board, moveList, c);

        for (PieceType pt = KNIGHT; pt <= KING; pt++)
        {
            pieceCaptures(board, moveList, c, pt);
        }
    }
    else
    {
        pieceCaptures(board, moveList, c, KING);
    }
}

inline void generateAllMoves(const Board *board, MoveList *moveList, const Color c) {
    generateAllQuiets(board, moveList, c);
    generateAllCaptures(board, moveList, c);
}