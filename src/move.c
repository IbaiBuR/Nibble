#include "move.h"

#include <stdio.h>
#include <string.h>

#include "bitboard.h"
#include "board.h"
#include "movegen.h"
#include "piece.h"

bool isPromotion(const Move move) { return flag(move) >= KNIGHT_PROMO; }

bool isCapture(const Move move) {
    return flag(move) == CAPTURE || flag(move) >= KNIGHT_CAPTURE_PROMO || flag(move) == ENPASSANT;
}

bool isEnPassant(const Move move) { return flag(move) == ENPASSANT; }
bool isDoublePush(const Move move) { return flag(move) == DOUBLEPUSH; }
bool isCastling(const Move move) { return flag(move) == CASTLE; }

inline int makeMove(const Move move, Board *board) {
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
        board->epSq = to - offset;

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
        board->fullMoveNumber++;

    if (pieceToPieceType[movingPiece] == PAWN)
        board->fmr = 0;

    board->occupancies[COLOR_NB] = board->occupancies[WHITE] | board->occupancies[BLACK];

    board->stm ^= 1;
    board->checkers = attacksToKing(board, lsbIndex(pieceBB(board, KING, board->stm)), board->stm);

    if (attackedBySide(board, lsbIndex(pieceBB(board, KING, board->stm ^ 1)), board->stm))
    {
        undoMove(board);
        return 0;
    }

    return 1;
}

void undoMove(Board *board) {
    board->histPly--;
    restoreBoardState(board);
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

Move parseMove(const char *move, const Board *board) {
    MoveList moveList;
    generateAllMoves(board, &moveList, board->stm);

    for (uint32_t i = 0; i < moveList.count; i++)
    {
        const Move currentMove = moveList.moves[i].move;

        if (strcmp(move, moveToString(currentMove)) == 0)
            return currentMove;
    }

    printf(" Illegal move.\n");
    return NOMOVE;
}