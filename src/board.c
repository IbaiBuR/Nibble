#include "board.h"

#include <stdio.h>
#include <string.h>

#include "bitboard.h"

void initializeBoard(Board *board) {
    memset(board->pieceBB, 0ULL, sizeof(board->pieceBB));
    memset(board->occupancies, 0ULL, sizeof(board->occupancies));

    board->checkers = 0ULL;
    board->pinned   = 0ULL;

    board->stm            = WHITE;
    board->epSq           = NO_SQ;
    board->castling       = 0;
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
    printf(" FiftyMoveRule  : %d\n", board.fmr);
    printf(" Move number    : %d\n\n", board.fullMoveNumber);
}

void parseFen(const char *fen, Board *board) {
    initializeBoard(board);

    for (Rank rank = RANK_1; rank < RANK_NB; rank++)
    {
        for (File file = A_FILE; file < FILE_NB; file++)
        {
            const Square sq = squareOf(file, rank);

            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                const Piece currentPiece = charToPiece[(int)*fen];
                setBit(board->pieceBB[currentPiece], sq);
                fen++;
            }
            if (*fen >= '0' && *fen <= '9')
            {
                Piece currentPiece = NO_PIECE;

                for (Piece bbPiece = W_PAWN; bbPiece < PIECE_NB; bbPiece++)
                    if (getBit(board->pieceBB[bbPiece], sq))
                        currentPiece = bbPiece;

                if (currentPiece == NO_PIECE)
                    file--;

                file += (*fen - '0'); // skip empty squares
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

Piece pieceOnSquare(const Board board, const Square sq) {
    Piece piece = NO_PIECE;

    for (Piece currentPiece = W_PAWN; currentPiece < PIECE_NB; currentPiece++)
    {
        if (getBit(board.pieceBB[currentPiece], sq))
        {
            piece = currentPiece;
            break;
        }
    }

    return piece;
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
         | (getBishopAttacks(kingSq, board->occupancies[c ^ 1]) & bishopsQueens)
         | (getRookAttacks(kingSq, board->occupancies[c ^ 1]) & rooksQueens);
}
