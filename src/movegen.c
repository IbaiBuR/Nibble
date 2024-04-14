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

void printMoveList(const MoveList moveList) {
    for (uint32_t i = 0; i < moveList.count; i++)
    {
        printf("%3d. Move: %-5s Score: %-4d\n", i + 1, moveToString(moveList.moves[i].move),
               moveList.moves[i].score);
    }
}

static inline void
generatePawnPushes(const Board *board, MoveList *moveList, const Color c, const Bitboard empty) {
    const Bitboard  pawns  = pieceBB(board, PAWN, c);
    const Direction offset = c == WHITE ? SOUTH : NORTH;

    Bitboard singlePush = singlePawnPush(pawns, empty, c);
    Bitboard doublePush = doublePawnPush(pawns, empty, c);

    while (singlePush)
    {
        const Square to         = popLsb(&singlePush);
        const Rank   movingRank = rankOf(to);

        if (movingRank != RANK_1 && movingRank != RANK_8)
        {
            addMove(buildMove(to - offset, to, QUIET), moveList);
        }
        else
        {
            addMove(buildMove(to - offset, to, KNIGHT_PROMO), moveList);
            addMove(buildMove(to - offset, to, BISHOP_PROMO), moveList);
            addMove(buildMove(to - offset, to, ROOK_PROMO), moveList);
            addMove(buildMove(to - offset, to, QUEEN_PROMO), moveList);
        }
    }

    while (doublePush)
    {
        const Square to = popLsb(&doublePush);
        addMove(buildMove(to - offset * 2, to, DOUBLEPUSH), moveList);
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
            const Square to         = popLsb(&possiblePawnCaptures);
            const Rank   movingRank = rankOf(to);

            if (movingRank != RANK_1 && movingRank != RANK_8)
            {
                addMove(buildMove(from, to, CAPTURE), moveList);
            }
            else
            {
                addMove(buildMove(from, to, KNIGHT_CAPTURE_PROMO), moveList);
                addMove(buildMove(from, to, BISHOP_CATURE_PROMO), moveList);
                addMove(buildMove(from, to, ROOK_CAPTURE_PROMO), moveList);
                addMove(buildMove(from, to, QUEEN_CAPTURE_PROMO), moveList);
            }
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
            getAttacksByPieceType(pt, from, board->occupancies[COLOR_NB])
            & board->occupancies[c ^ 1];

        while (possiblePieceCaptures)
        {
            addMove(buildMove(from, popLsb(&possiblePieceCaptures), CAPTURE), moveList);
        }
    }
}

static inline void generateCastlingMoves(const Board *board, MoveList *moveList, const Color c) {
    const Bitboard occupied = board->occupancies[COLOR_NB];

    if (c == WHITE)
    {
        if (board->castling & WK && !getBit(occupied, G1) && !getBit(occupied, F1)
            && !attackedBySide(board, F1, BLACK))
        {
            addMove(buildMove(E1, G1, CASTLE), moveList);
        }
        if (board->castling & WQ && !getBit(occupied, D1) && !getBit(occupied, C1)
            && !getBit(occupied, B1) && !attackedBySide(board, D1, BLACK))
        {
            addMove(buildMove(E1, C1, CASTLE), moveList);
        }
    }
    else
    {
        if (board->castling & BK && !getBit(occupied, G8) && !getBit(occupied, F8)
            && !attackedBySide(board, F8, WHITE))
        {
            addMove(buildMove(E8, G8, CASTLE), moveList);
        }
        if (board->castling & BQ && !getBit(occupied, D8) && !getBit(occupied, C8)
            && !getBit(occupied, B8) && !attackedBySide(board, D8, WHITE))
        {
            addMove(buildMove(E8, C8, CASTLE), moveList);
        }
    }
}

inline void generateAllQuiets(const Board *board, MoveList *moveList, const Color c) {

    for (PieceType pt = KNIGHT; pt <= KING; pt++)
    {
        quietPieceMoves(board, moveList, c, pt, ~board->occupancies[COLOR_NB]);
    }
}

inline void generateAllCaptures(const Board *board, MoveList *moveList, const Color c) {
    pawnCaptures(board, moveList, c);

    for (PieceType pt = KNIGHT; pt <= KING; pt++)
    {
        pieceCaptures(board, moveList, c, pt);
    }
}

inline void generateAllMoves(const Board *board, MoveList *moveList, const Color c) {
    moveList->count = 0;

    if (bitCount(board->checkers) == 0)
    {
        generateAllCaptures(board, moveList, c);
        generatePawnPushes(board, moveList, c, ~board->occupancies[COLOR_NB]);
        generateAllQuiets(board, moveList, c);
        generateCastlingMoves(board, moveList, c);
    }
    else if (bitCount(board->checkers) == 1)
    {
        generateAllCaptures(board, moveList, c);
        generatePawnPushes(board, moveList, c, ~board->occupancies[COLOR_NB]);
        generateAllQuiets(board, moveList, c);
    }
    else
    {
        quietPieceMoves(board, moveList, c, KING, ~board->occupancies[COLOR_NB]);
        pieceCaptures(board, moveList, c, KING);
    }
}