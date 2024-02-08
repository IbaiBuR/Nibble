#include "magics.h"

#include "bitboard.h"

void initSlidersAttacks(PieceType pt) {
    for (Square sq = A8; sq < SQUARE_NB; sq++)
    {
        Bitboard attackMask = pt == BISHOP ? bishopMasks[sq] : rookMasks[sq];
        int      nBits      = bitCount(attackMask);
        int      occupancyIndex = (1 << nBits);

        for (int i = 0; i < occupancyIndex; i++)
        {
            if (pt == BISHOP)
            {
                Bitboard occupancy  = setOccupancies(i, nBits, attackMask);
                int      magicIndex = (occupancy * bishopMagics[sq])
                              >> (64 - bishopRelevantOccBits[sq]);
                bishopAttacks[sq][magicIndex] = genBishopAttacks(sq, occupancy);
            }
            else
            {
                Bitboard occupancy  = setOccupancies(i, nBits, attackMask);
                int      magicIndex = (occupancy * rookMagics[sq])
                              >> (64 - rookRelevantOccBits[sq]);
                rookAttacks[sq][magicIndex] = genRookAttacks(sq, occupancy);
            }
        }
    }
}
