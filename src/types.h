#pragma once

#include <inttypes.h>

#define MAX_MOVES 256

#define INLINE static inline __attribute__((always_inline))

#define squareOf(file, rank) ((rank << 3) + file)
#define fileOf(square)       (square & 7)
#define rankOf(square)       (square >> 3)

typedef uint64_t Bitboard;
typedef uint32_t Move;
typedef int32_t  Score;

// clang-format off
typedef enum Square : int {
    A8, B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1,
    SQUARE_NB, NO_SQ
} Square;
// clang-format on

typedef enum File : int {
    A_FILE,
    B_FILE,
    C_FILE,
    D_FILE,
    E_FILE,
    F_FILE,
    G_FILE,
    H_FILE,
    FILE_NB
} File;

typedef enum Rank : int {
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_NB
} Rank;

enum Direction : int {
    NORTH      = 8,
    SOUTH      = -NORTH,
    EAST       = 1,
    WEST       = -EAST,
    NORTH_EAST = NORTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST
};

typedef enum Color : int {
    WHITE,
    BLACK,
    COLOR_NB
} Color;

typedef enum PieceType : int {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

typedef enum Piece : int {
    W_PAWN,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,
    B_PAWN,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,
    PIECE_NB,
    NO_PIECE = -1
} Piece;

enum castlingMask : int {
    WK = 1, // 0001 white can castle kingside
    WQ = 2, // 0010 white can castle queenside
    BK = 4, // 0100 black can castle kingside
    BQ = 8, // 1000 black can castle queenside
};

typedef struct Board {
    Bitboard pieceBB[PIECE_NB];         // bitboards for each piece type
    Bitboard occupancies[COLOR_NB + 1]; // occupancies for white, black and both
    int      stm;                       // side to move
    int      epSq;                      // en-passant square
    int      castling;                  // castling mask
    int      fmr;                       // halfmoves till fifty-move rule
    int      fullMoveNumber;            // number of fullmoves
} Board;

typedef struct ScoredMove {
    Move  move;
    Score score;
} ScoredMove;

typedef struct MoveList {
    uint32_t count;
    Move     moves[MAX_MOVES];
} MoveList;
