#include "move.h"

#include <stdio.h>

#include "board.h"

bool isPromotion(const Move move) {
    const int moveFlag = flag(move);

    return moveFlag == KNIGHT_PROMO || moveFlag == BISHOP_PROMO || moveFlag == ROOK_PROMO
        || moveFlag == QUEEN_PROMO;
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