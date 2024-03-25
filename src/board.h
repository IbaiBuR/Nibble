#pragma once

#include "types.h"

#define startPosFen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

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

constexpr char pieceToChar[PIECE_NB] = "PNBRQKpnbrqk";

constexpr Piece charToPiece[] = {
    ['P'] = W_PAWN,   ['N'] = W_KNIGHT, ['B'] = W_BISHOP, ['R'] = W_ROOK,
    ['Q'] = W_QUEEN,  ['K'] = W_KING,   ['p'] = B_PAWN,   ['n'] = B_KNIGHT,
    ['b'] = B_BISHOP, ['r'] = B_ROOK,   ['q'] = B_QUEEN,  ['k'] = B_KING};

void initializeBoard(Board *board);
void printBoard(Board board);
void parseFen(const char *fen, Board *board);

Piece pieceOnSquare(Board board, Square square);

Bitboard pieceBB(const Board *board, PieceType pieceType, Color color);
Bitboard attacksToKing(const Board *board, Square square, Color color);
