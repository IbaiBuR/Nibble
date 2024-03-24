#pragma once

#include "types.h"

#define buildMove(from, to, flag) ((from) | (to << 6) | (flag << 12))
#define from(move)                ((move & 0x3F))
#define to(move)                  (((move >> 6) & 0x3F))
#define flag(move)                (((move >> 12) & 0x0F))

enum MoveFlags {
    NOMOVE,
    QUIET,
    CAPTURE,
    ENPASSANT,
    CASTLE,
    DOUBLEPUSH,
    KNIGHT_PROMO,
    BISHOP_PROMO,
    ROOK_PROMO,
    QUEEN_PROMO
};

constexpr char promoFlagToChar[] = {
    [KNIGHT_PROMO] = 'n', [BISHOP_PROMO] = 'b', [ROOK_PROMO] = 'r', [QUEEN_PROMO] = 'q'};

bool isPromotion(Move move);

char *moveToString(Move move);
