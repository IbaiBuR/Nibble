#include "move.h"

#include <stdio.h>

#include "bitboard.h"
#include "board.h"
#include "piece.h"

bool isPromotion(const Move move) { return flag(move) >= KNIGHT_PROMO; }

bool isCapture(const Move move) {
    return flag(move) == CAPTURE || flag(move) >= KNIGHT_CAPTURE_PROMO || flag(move) == ENPASSANT;
}

bool isEnPassant(const Move move) { return flag(move) == ENPASSANT; }
bool isDoublePush(const Move move) { return flag(move) == DOUBLEPUSH; }
bool isCastling(const Move move) { return flag(move) == CASTLE; }

inline void makeMove(const Move move, Board *board) {
    copyBoardState(board);

    board->histPly++;
    board->fmr++;

    const Square    from        = from(move);
    const Square    to          = to(move);
    const Piece     movingPiece = pieceOnSquare(board, from);
    const Direction offset      = board->stm == WHITE ? SOUTH : NORTH;

    if (isCapture(move))
    {
        const Square targetSquare = isEnPassant(move) ? board->epSq - offset : to;
        removePiece(board, pieceOnSquare(board, targetSquare), targetSquare);
        board->fmr = 0;
    }

    removePiece(board, movingPiece, from);
    addPiece(board,
             isPromotion(move) ? promotionToPieceType[flag(move)] + board->stm * 6 : movingPiece,
             to);

    board->epSq = NO_SQ;

    if (isDoublePush(move))
    {
        board->epSq = to - offset;
    }

    if (isCastling(move))
    {
        switch (to)
        {
        case G1:
            movePiece(board, pieceOnSquare(board, H1), H1, F1);
            break;
        case C1:
            movePiece(board, pieceOnSquare(board, A1), A1, D1);
            break;
        case G8:
            movePiece(board, pieceOnSquare(board, H8), H8, F8);
            break;
        case C8:
            movePiece(board, pieceOnSquare(board, A8), A8, D8);
            break;
        default:
            printf("Castling to invalid square!\n");
            break;
        }
    }

    board->castling &= castlingRights[from];
    board->castling &= castlingRights[to];

    if (board->stm == BLACK)
    {
        board->fullMoveNumber++;
    }

    board->stm ^= 1;
    board->checkers = attacksToKing(board, lsbIndex(pieceBB(board, KING, board->stm)), board->stm);
}

char *moveToString(const Move move) {
    static char buffer[6];

    if (isPromotion(move))
    {
        sprintf(buffer, "%s%s%c", squareToCoordinates[from(move)], squareToCoordinates[to(move)],
                promoFlagToChar[flag(move)]);
    }
    else
    {
        sprintf(buffer, "%s%s", squareToCoordinates[from(move)], squareToCoordinates[to(move)]);
    }

    return buffer;
}