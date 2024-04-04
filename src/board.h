#pragma once

#include "types.h"

#define startPosFen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define kiwipete    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"

#define squareOf(file, rank) ((rank << 3) + file)
#define fileOf(square)       (square & 7)
#define rankOf(square)       (square >> 3)

// clang-format off
constexpr SquareName squareToCoordinates[SQUARE_NB] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};
// clang-format on

constexpr CastlingRightsStr castlingRightsToString[16] = {
    [0] = "-",   [1] = "K",    [2] = "Q",    [3] = "KQ",   [4] = "k",   [5] = "Kk",
    [6] = "Qk",  [7] = "KQk",  [8] = "q",    [9] = "Kq",   [10] = "Qq", [11] = "KQq",
    [12] = "kq", [13] = "Kkq", [14] = "Qkq", [15] = "KQkq"};

void initializeBoard(Board *board);
void printBoard(Board board);
void parseFen(const char *fen, Board *board);

char *boardToFen(char *fenBuffer, const Board *board);

void copyBoardState(Board *board);
void restoreBoardState(Board *board);

bool attackedBySide(const Board *board, Square square, Color color);

Bitboard pieceBB(const Board *board, PieceType pieceType, Color color);
Bitboard attacksToKing(const Board *board, Square square, Color color);
