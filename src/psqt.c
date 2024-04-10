#include "psqt.h"

#include "piece.h"

static const int *mgPieceTables[PIECETYPE_NB] = {mgPawnTable, mgKnightTable, mgBishopTable,
                                                 mgRookTable, mgQueenTable,  mgKingTable};

static const int *egPieceTables[PIECETYPE_NB] = {egPawnTable, egKnightTable, egBishopTable,
                                                 egRookTable, egQueenTable,  egKnightTable};

int mgTable[PIECE_NB][SQUARE_NB];
int egTable[PIECE_NB][SQUARE_NB];

inline void initPSQT() {
    for (Piece p = W_PAWN; p < PIECE_NB; p++)
    {
        for (Square sq = A8; sq < SQUARE_NB; sq++)
        {
            const PieceType pt = pieceToPieceType[p];

            if (pieceColor[p] == WHITE)
            {
                mgTable[p][sq] = mgPieceValues[p] + mgPieceTables[pt][sq];
                egTable[p][sq] = egPieceValues[p] + egPieceTables[pt][sq];
            }
            else
            {
                mgTable[p][sq] = mgPieceValues[p] - mgPieceTables[pt][sq ^ 56];
                egTable[p][sq] = egPieceValues[p] - egPieceTables[pt][sq ^ 56];
            }
        }
    }
}
