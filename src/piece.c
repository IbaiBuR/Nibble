#include "piece.h"

#include "bitboard.h"

void movePiece(Board *board, const Piece piece, const Square from, const Square to) {
    resetBit(board->pieceBB[piece], from);
    resetBit(board->occupancies[pieceColor[piece]], from);
    setBit(board->pieceBB[piece], to);
    setBit(board->occupancies[pieceColor[piece]], to);
}

Piece pieceOnSquare(const Board *board, const Square sq) {
    Piece piece = NO_PIECE;

    for (Piece currentPiece = W_PAWN; currentPiece < PIECE_NB; currentPiece++)
    {
        if (getBit(board->pieceBB[currentPiece], sq))
        {
            piece = currentPiece;
            break;
        }
    }

    return piece;
}