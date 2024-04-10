#include "eval.h"

#include "bitboard.h"
#include "psqt.h"
#include "util.h"

Score evaluate(const Board *board) {
    Score mgScore = 0, egScore = 0;
    int   mgPhase = 0;

    for (Piece bbPiece = W_PAWN; bbPiece < PIECE_NB; bbPiece++)
    {
        Bitboard currentPieceBB = board->pieceBB[bbPiece];

        while (currentPieceBB)
        {
            const Square pieceIndex = popLsb(&currentPieceBB);

            mgScore += mgTable[bbPiece][pieceIndex];
            egScore += egTable[bbPiece][pieceIndex];
            mgPhase += gamePhaseInc[bbPiece];
        }
    }

    mgPhase          = MIN(mgPhase, maxGamePhase);
    const Score eval = (mgScore * mgPhase + egScore * (maxGamePhase - mgPhase)) / maxGamePhase;

    return board->stm == WHITE ? eval : -eval;
}
