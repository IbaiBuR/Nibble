#include "board.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "bitboard.h"
#include "piece.h"

void initializeBoard(Board *board) {
    memset(board->pieceBB, 0ULL, sizeof(board->pieceBB));
    memset(board->occupancies, 0ULL, sizeof(board->occupancies));

    board->checkers = 0ULL;

    board->stm            = WHITE;
    board->epSq           = NO_SQ;
    board->castling       = 0;
    board->histPly        = 0;
    board->fmr            = 0;
    board->fullMoveNumber = 1;
}

void printBoard(const Board board) {
    printf("\n +---+---+---+---+---+---+---+---+\n");

    for (Rank rank = RANK_1; rank < RANK_NB; rank++)
    {
        for (File file = A_FILE; file < FILE_NB; file++)
        {
            Piece currentPiece = NO_PIECE;

            for (Piece bbPiece = W_PAWN; bbPiece < PIECE_NB; bbPiece++)
                if (getBit(board.pieceBB[bbPiece], squareOf(file, rank)))
                    currentPiece = bbPiece;

            printf(" | %c", (currentPiece == NO_PIECE) ? ' ' : pieceToChar[currentPiece]);
        }
        printf(" | %d\n +---+---+---+---+---+---+---+---+\n", 8 - rank);
    }

    printf("   A   B   C   D   E   F   G   H\n\n");
    printf(" Side to move   : %s\n", (board.stm == WHITE) ? "white" : "black");
    printf(" Enpassant      : %s\n",
           (board.epSq != NO_SQ) ? squareToCoordinates[board.epSq] : "no");
    // clang-format off
    printf(" Castling rights: %c%c%c%c\n",
           (board.castling & WK) ? 'K' : '-',
           (board.castling & WQ) ? 'Q' : '-',
           (board.castling & BK) ? 'k' : '-',
           (board.castling & BQ) ? 'q' : '-');
    // clang-format on

    char fenBuffer[128] = {};

    printf(" FiftyMoveRule  : %d\n", board.fmr);
    printf(" Move number    : %d\n", board.fullMoveNumber);
    printf(" FEN            : %s\n\n", boardToFen(fenBuffer, &board));
}

void parseFen(const char *fen, Board *board) {
    initializeBoard(board);

    for (Rank rank = RANK_1; rank < RANK_NB; rank++)
    {
        for (File file = A_FILE; file < FILE_NB; file++)
        {
            const Square sq = squareOf(file, rank);

            if (isalpha(*fen))
            {
                const Piece currentPiece = charToPiece[(int)*fen];
                setBit(board->pieceBB[currentPiece], sq);
                fen++;
            }
            if (isdigit(*fen))
            {
                Piece currentPiece = NO_PIECE;

                for (Piece bbPiece = W_PAWN; bbPiece < PIECE_NB; bbPiece++)
                    if (getBit(board->pieceBB[bbPiece], sq))
                        currentPiece = bbPiece;

                if (currentPiece == NO_PIECE)
                    file--;

                file += (*fen - '0'); // Skip empty squares
                fen++;
            }
            if (*fen == '/')
                fen++;
        }
    }
    fen++;
    board->stm = (*fen == 'w') ? WHITE : BLACK;

    fen += 2;

    while (*fen != ' ')
    {
        switch (*fen)
        {
        case 'K':
            board->castling |= WK;
            break;
        case 'Q':
            board->castling |= WQ;
            break;
        case 'k':
            board->castling |= BK;
            break;
        case 'q':
            board->castling |= BQ;
            break;
        default:
            break;
        }
        fen++;
    }

    fen++;
    board->epSq = *fen != '-' ? squareOf(fen[0] - 'a', (8 - (fen[1] - '0'))) : NO_SQ;

    while (*fen && *fen != ' ')
        fen++;

    sscanf(fen, " %d %d", &board->fmr, &board->fullMoveNumber);

    for (Piece piece = W_PAWN; piece <= W_KING; piece++)
        board->occupancies[WHITE] |= board->pieceBB[piece];

    for (Piece piece = B_PAWN; piece <= B_KING; piece++)
        board->occupancies[BLACK] |= board->pieceBB[piece];

    board->occupancies[COLOR_NB] |= board->occupancies[WHITE];
    board->occupancies[COLOR_NB] |= board->occupancies[BLACK];

    board->checkers |= attacksToKing(board, lsbIndex(pieceBB(board, KING, board->stm)), board->stm);
}

char *boardToFen(char *fenBuffer, const Board *board) {
    char *reference = fenBuffer;

    for (Rank rank = RANK_1; rank < RANK_NB; rank++)
    {
        unsigned emptySquares = 0;

        for (File file = A_FILE; file < FILE_NB; file++)
        {
            const Square sq    = squareOf(file, rank);
            const Piece  piece = pieceOnSquare(board, sq);

            if (piece != NO_PIECE)
            {
                if (emptySquares)
                    *fenBuffer++ = '0' + emptySquares;
                *fenBuffer++ = pieceToChar[piece];
                emptySquares = 0;
            }
            else
                emptySquares++;
        }

        if (emptySquares)
            *fenBuffer++ = '0' + emptySquares;

        *fenBuffer++ = rank == RANK_8 ? ' ' : '/';
    }

    sprintf(fenBuffer, "%s %s %s %d %d", board->stm ? "b" : "w",
            castlingRightsToString[board->castling],
            board->epSq != NO_SQ ? squareToCoordinates[board->epSq] : "-", board->fmr,
            board->fullMoveNumber);

    return reference;
}

void copyBoardState(Board *board) {
    memcpy(&board->history[board->histPly].pieceCopy, &board->pieceBB, sizeof(board->pieceBB));
    memcpy(&board->history[board->histPly].occupancyCopy, &board->occupancies,
           sizeof(board->occupancies));

    board->history[board->histPly].checkers       = board->checkers;
    board->history[board->histPly].stm            = board->stm;
    board->history[board->histPly].epSq           = board->epSq;
    board->history[board->histPly].castling       = board->castling;
    board->history[board->histPly].fmr            = board->fmr;
    board->history[board->histPly].fullMoveNumber = board->fullMoveNumber;
}

void restoreBoardState(Board *board) {
    memcpy(&board->pieceBB, &board->history[board->histPly].pieceCopy, sizeof(board->pieceBB));
    memcpy(&board->occupancies, &board->history[board->histPly].occupancyCopy,
           sizeof(board->occupancies));

    board->checkers       = board->history[board->histPly].checkers;
    board->stm            = board->history[board->histPly].stm;
    board->epSq           = board->history[board->histPly].epSq;
    board->castling       = board->history[board->histPly].castling;
    board->fmr            = board->history[board->histPly].fmr;
    board->fullMoveNumber = board->history[board->histPly].fullMoveNumber;
}

inline bool attackedBySide(const Board *board, const Square sq, const Color c) {
    const Bitboard occupancies = board->occupancies[COLOR_NB];

    if (pawnAttacks[c ^ 1][sq] & pieceBB(board, PAWN, c))
        return true;
    if (knightAttacks[sq] & pieceBB(board, KNIGHT, c))
        return true;
    if (kingAttacks[sq] & pieceBB(board, KING, c))
        return true;
    if (getBishopAttacks(sq, occupancies) & (pieceBB(board, BISHOP, c) | pieceBB(board, QUEEN, c)))
        return true;
    if (getRookAttacks(sq, occupancies) & (pieceBB(board, ROOK, c) | pieceBB(board, QUEEN, c)))
        return true;

    return false;
}

inline Bitboard pieceBB(const Board *board, const PieceType pt, const Color c) {
    return board->pieceBB[pt + c * 6];
}

Bitboard attacksToKing(const Board *board, const Square kingSq, const Color c) {
    const Bitboard oppPawns      = pieceBB(board, PAWN, c ^ 1);
    const Bitboard oppKnights    = pieceBB(board, KNIGHT, c ^ 1);
    const Bitboard bishopsQueens = pieceBB(board, BISHOP, c ^ 1) | pieceBB(board, QUEEN, c ^ 1);
    const Bitboard rooksQueens   = pieceBB(board, ROOK, c ^ 1) | pieceBB(board, QUEEN, c ^ 1);

    return (pawnAttacks[c][kingSq] & oppPawns) | (knightAttacks[kingSq] & oppKnights)
         | (getBishopAttacks(kingSq, board->occupancies[COLOR_NB]) & bishopsQueens)
         | (getRookAttacks(kingSq, board->occupancies[COLOR_NB]) & rooksQueens);
}
