#include "bitboard.h"

#include <inttypes.h>
#include <stdio.h>

#include "board.h"
#include "types.h"

Bitboard pawnAttacks[COLOR_NB][SQUARE_NB];
Bitboard knightAttacks[SQUARE_NB];
Bitboard kingAttacks[SQUARE_NB];
Bitboard bishopMasks[SQUARE_NB];
Bitboard rookMasks[SQUARE_NB];
Bitboard bishopAttacks[SQUARE_NB][512];
Bitboard rookAttacks[SQUARE_NB][4096];

Bitboard genPawnAttacks(const Square sq, const Color c) {
    Bitboard bb      = 0ULL;
    Bitboard attacks = 0ULL;

    setBit(bb, sq);

    if (c == WHITE)
    {
        attacks |= shiftNW(bb);
        attacks |= shiftNE(bb);
    }
    else
    {
        attacks |= shiftSW(bb);
        attacks |= shiftSE(bb);
    }

    return attacks;
}

Bitboard genKnightAttacks(const Square sq) {
    Bitboard bb      = 0ULL;
    Bitboard attacks = 0ULL;

    setBit(bb, sq);

    // white's knight attacks
    if ((bb >> 17) & NOT_H_FILE)
        attacks |= (bb >> 17);
    if ((bb >> 15) & NOT_A_FILE)
        attacks |= (bb >> 15);
    if ((bb >> 10) & NOT_HG_FILE)
        attacks |= (bb >> 10);
    if ((bb >> 6) & NOT_AB_FILE)
        attacks |= (bb >> 6);

    // black's knight attacks
    if ((bb << 17) & NOT_A_FILE)
        attacks |= (bb << 17);
    if ((bb << 15) & NOT_H_FILE)
        attacks |= (bb << 15);
    if ((bb << 10) & NOT_AB_FILE)
        attacks |= (bb << 10);
    if ((bb << 6) & NOT_HG_FILE)
        attacks |= (bb << 6);

    return attacks;
}

Bitboard genKingAttacks(const Square sq) {
    Bitboard bb      = 0ULL;
    Bitboard attacks = 0ULL;

    setBit(bb, sq);

    // king can move 1 square in all directions
    attacks |= shiftN(bb);
    attacks |= shiftS(bb);
    attacks |= shiftE(bb);
    attacks |= shiftW(bb);
    attacks |= shiftNE(bb);
    attacks |= shiftNW(bb);
    attacks |= shiftSE(bb);
    attacks |= shiftSW(bb);

    return attacks;
}

Bitboard genBishopMasks(const Square sq) {
    Bitboard attacks = 0ULL;
    Rank     initialRank;
    File     initialFile;

    const Rank targetRank = rankOf(sq);
    const File targetFile = fileOf(sq);

    for (initialRank = targetRank + 1, initialFile = targetFile + 1;
         initialRank <= 6 && initialFile <= 6; initialRank++, initialFile++)
        attacks |= (bitOf(squareOf(initialFile, initialRank)));
    for (initialRank = targetRank + 1, initialFile = targetFile - 1;
         initialRank <= 6 && initialFile >= 1; initialRank++, initialFile--)
        attacks |= (bitOf(squareOf(initialFile, initialRank)));
    for (initialRank = targetRank - 1, initialFile = targetFile + 1;
         initialRank >= 1 && initialFile <= 6; initialRank--, initialFile++)
        attacks |= (bitOf(squareOf(initialFile, initialRank)));
    for (initialRank = targetRank - 1, initialFile = targetFile - 1;
         initialRank >= 1 && initialFile >= 1; initialRank--, initialFile--)
        attacks |= (bitOf(squareOf(initialFile, initialRank)));

    return attacks;
}

Bitboard genRookMasks(const Square sq) {
    Bitboard attacks = 0ULL;
    Rank     initialRank;
    File     initialFile;

    const Rank targetRank = rankOf(sq);
    const File targetFile = fileOf(sq);

    for (initialRank = targetRank + 1; initialRank <= 6; initialRank++)
        attacks |= (bitOf(squareOf(targetFile, initialRank)));
    for (initialRank = targetRank - 1; initialRank >= 1; initialRank--)
        attacks |= (bitOf(squareOf(targetFile, initialRank)));
    for (initialFile = targetFile + 1; initialFile <= 6; initialFile++)
        attacks |= (bitOf(squareOf(initialFile, targetRank)));
    for (initialFile = targetFile - 1; initialFile >= 1; initialFile--)
        attacks |= (bitOf(squareOf(initialFile, targetRank)));

    return attacks;
}

Bitboard genBishopAttacks(const Square sq, const Bitboard blockers) {
    Bitboard attacks = 0ULL;
    Rank     initialRank;
    File     initialFile;

    const Rank targetRank = rankOf(sq);
    const File targetFile = fileOf(sq);

    for (initialRank = targetRank + 1, initialFile = targetFile + 1;
         initialRank <= 7 && initialFile <= 7; initialRank++, initialFile++)
    {
        attacks |= (bitOf(squareOf(initialFile, initialRank)));
        if ((bitOf(squareOf(initialFile, initialRank))) & blockers)
            break;
    }
    for (initialRank = targetRank + 1, initialFile = targetFile - 1;
         initialRank <= 7 && initialFile >= 0; initialRank++, initialFile--)
    {
        attacks |= (bitOf(squareOf(initialFile, initialRank)));
        if ((bitOf(squareOf(initialFile, initialRank))) & blockers)
            break;
    }
    for (initialRank = targetRank - 1, initialFile = targetFile + 1;
         initialRank >= 0 && initialFile <= 7; initialRank--, initialFile++)
    {
        attacks |= (bitOf(squareOf(initialFile, initialRank)));
        if ((bitOf(squareOf(initialFile, initialRank))) & blockers)
            break;
    }
    for (initialRank = targetRank - 1, initialFile = targetFile - 1;
         initialRank >= 0 && initialFile >= 0; initialRank--, initialFile--)
    {
        attacks |= (bitOf(squareOf(initialFile, initialRank)));
        if ((bitOf(squareOf(initialFile, initialRank))) & blockers)
            break;
    }

    return attacks;
}

Bitboard genRookAttacks(const Square sq, const Bitboard blockers) {
    Bitboard attacks = 0ULL;
    Rank     initialRank;
    File     initialFile;

    const Rank targetRank = rankOf(sq);
    const File targetFile = fileOf(sq);

    for (initialRank = targetRank + 1; initialRank <= 7; initialRank++)
    {
        attacks |= (bitOf(squareOf(targetFile, initialRank)));
        if ((bitOf(squareOf(targetFile, initialRank))) & blockers)
            break;
    }
    for (initialRank = targetRank - 1; initialRank >= 0; initialRank--)
    {
        attacks |= (bitOf(squareOf(targetFile, initialRank)));
        if ((bitOf(squareOf(targetFile, initialRank))) & blockers)
            break;
    }
    for (initialFile = targetFile + 1; initialFile <= 7; initialFile++)
    {
        attacks |= (bitOf(squareOf(initialFile, targetRank)));
        if ((bitOf(squareOf(initialFile, targetRank))) & blockers)
            break;
    }
    for (initialFile = targetFile - 1; initialFile >= 0; initialFile--)
    {
        attacks |= (bitOf(squareOf(initialFile, targetRank)));
        if ((bitOf(squareOf(initialFile, targetRank))) & blockers)
            break;
    }

    return attacks;
}

Bitboard setOccupancies(const int index, const int nBits, Bitboard attacks) {
    Bitboard occupancies = 0ULL;

    for (int count = 0; count < nBits; count++)
    {
        const Square sq = lsbIndex(attacks);
        resetBit(attacks, sq);

        if (index & (1 << count))
            occupancies |= (1ULL << sq);
    }

    return occupancies;
}

Bitboard getAttacksByPieceType(const PieceType pt, const Square sq, const Bitboard blockers) {
    switch (pt)
    {
    case PAWN:
        return pawnAttacks[sq];
        break;
    case KNIGHT:
        return knightAttacks[sq];
        break;
    case KING:
        return kingAttacks[sq];
        break;
    case BISHOP:
        return getBishopAttacks(sq, blockers);
        break;
    case ROOK:
        return getRookAttacks(sq, blockers);
        break;
    case QUEEN:
        return getBishopAttacks(sq, blockers) | getRookAttacks(sq, blockers);
        break;
    default:
        printf("Invalid piece type.\n");
        return 0ULL;
        break;
    }
}

inline void initBB() {
    for (Square sq = A8; sq < SQUARE_NB; sq++)
    {
        pawnAttacks[WHITE][sq] = genPawnAttacks(sq, WHITE);
        pawnAttacks[BLACK][sq] = genPawnAttacks(sq, BLACK);
        knightAttacks[sq]      = genKnightAttacks(sq);
        kingAttacks[sq]        = genKingAttacks(sq);
        bishopMasks[sq]        = genBishopMasks(sq);
        rookMasks[sq]          = genRookMasks(sq);
    }

    initSlidersAttacks(BISHOP);
    initSlidersAttacks(ROOK);
}

inline void printBB(const Bitboard bitboard) {
    printf("\n");
    for (Rank rank = RANK_1; rank < RANK_NB; rank++)
    {
        for (File file = A_FILE; file < FILE_NB; file++)
        {
            if (!file)
                printf("  %d ", 8 - rank);
            printf(" %d", getBit(bitboard, squareOf(file, rank)) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n     a b c d e f g h\n\n");
    printf("     Bitboard: 0x%" PRIX64 "ULL\n\n", bitboard);
}

inline void initSlidersAttacks(const PieceType pt) {
    for (Square sq = A8; sq < SQUARE_NB; sq++)
    {
        const Bitboard attackMask     = pt == BISHOP ? bishopMasks[sq] : rookMasks[sq];
        const int      nBits          = bitCount(attackMask);
        const int      occupancyIndex = (1 << nBits);

        for (int i = 0; i < occupancyIndex; i++)
        {
            if (pt == BISHOP)
            {
                const Bitboard occupancy = setOccupancies(i, nBits, attackMask);
                const int      magicIndex =
                    (occupancy * bishopMagics[sq]) >> (64 - bishopRelevantOccBits[sq]);
                bishopAttacks[sq][magicIndex] = genBishopAttacks(sq, occupancy);
            }
            else
            {
                const Bitboard occupancy = setOccupancies(i, nBits, attackMask);
                const int      magicIndex =
                    (occupancy * rookMagics[sq]) >> (64 - rookRelevantOccBits[sq]);
                rookAttacks[sq][magicIndex] = genRookAttacks(sq, occupancy);
            }
        }
    }
}

Bitboard singlePawnPush(const Bitboard pawns, const Bitboard empty, const Color c) {
    return (c == WHITE) ? shiftN(pawns) & empty : shiftS(pawns) & empty;
}

Bitboard doublePawnPush(const Bitboard pawns, const Bitboard empty, const Color c) {
    return (c == WHITE) ? shiftN(singlePawnPush(pawns, empty, WHITE)) & empty & rank4
                        : shiftS(singlePawnPush(pawns, empty, BLACK)) & empty & rank5;
}