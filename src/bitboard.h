#pragma once

#include "magics.h"
#include "types.h"

#define NOT_A_FILE  0xFEFEFEFEFEFEFEFEULL
#define NOT_H_FILE  0x7F7F7F7F7F7F7F7FULL
#define NOT_AB_FILE 0xFCFCFCFCFCFCFCFCULL
#define NOT_HG_FILE 0x3F3F3F3F3F3F3F3FULL

#define rank1 0xFF00000000000000ULL
#define rank2 0x00FF000000000000ULL
#define rank3 0x0000FF0000000000ULL
#define rank4 0x000000FF00000000ULL
#define rank5 0x00000000FF000000ULL
#define rank6 0x0000000000FF0000ULL
#define rank7 0x000000000000FF00ULL
#define rank8 0x00000000000000FFULL

#define getBit(bitboard, square)   (bitboard & bitOf(square))
#define setBit(bitboard, square)   (bitboard |= bitOf(square))
#define resetBit(bitboard, square) (bitboard ^= bitOf(square) & bitboard)
#define bitCount(bitboard)         (__builtin_popcountll(bitboard))
#define lsbIndex(bb)               (__builtin_ctzll(bb))
#define msbIndex(bb)               (63 ^ __builtin_clzll(bb))

// clang-format off
// Relevant occupancy bits for bishops
static const int bishopRelevantOccBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

// Relevant occupancy bits for rooks
static const int rookRelevantOccBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
// clang-format on

INLINE Bitboard bitOf(Square sq) { return 1ULL << sq; }
INLINE Bitboard shiftN(Bitboard bb) { return bb >> 8; }
INLINE Bitboard shiftS(Bitboard bb) { return bb << 8; }
INLINE Bitboard shiftW(Bitboard bb) { return bb >> 1; }
INLINE Bitboard shiftE(Bitboard bb) { return bb << 1; }
INLINE Bitboard shiftNW(Bitboard bb) { return (bb >> 7) & NOT_A_FILE; }
INLINE Bitboard shiftNE(Bitboard bb) { return (bb >> 9) & NOT_H_FILE; }
INLINE Bitboard shiftSW(Bitboard bb) { return (bb << 7) & NOT_H_FILE; }
INLINE Bitboard shiftSE(Bitboard bb) { return (bb << 9) & NOT_A_FILE; }

extern Bitboard pawnAttacks[COLOR_NB][SQUARE_NB];
extern Bitboard knightAttacks[SQUARE_NB];
extern Bitboard kingAttacks[SQUARE_NB];
extern Bitboard bishopMasks[SQUARE_NB];
extern Bitboard rookMasks[SQUARE_NB];
extern Bitboard bishopAttacks[SQUARE_NB][512];
extern Bitboard rookAttacks[SQUARE_NB][4096];

Bitboard genPawnAttacks(Square square, Color color);
Bitboard genKnightAttacks(Square square);
Bitboard genKingAttacks(Square square);
Bitboard genBishopMasks(Square square);
Bitboard genRookMasks(Square square);
Bitboard genBishopAttacks(Square square, Bitboard blockers);
Bitboard genRookAttacks(Square square, Bitboard blockers);
Bitboard setOccupancies(int index, int nBits, Bitboard attacks);
Bitboard attacksToSq(Board *board, Square square, Bitboard occupancy);

INLINE Bitboard getBishopAttacks(Square square, Bitboard occupancy) {
    occupancy &= bishopMasks[square];
    occupancy *= bishopMagics[square];
    occupancy >>= 64 - bishopRelevantOccBits[square];

    return bishopAttacks[square][occupancy];
}

INLINE Bitboard getRookAttacks(Square square, Bitboard occupancy) {
    occupancy &= rookMasks[square];
    occupancy *= rookMagics[square];
    occupancy >>= 64 - rookRelevantOccBits[square];

    return rookAttacks[square][occupancy];
}

void initBB();
void printBB(Bitboard bitboard);
