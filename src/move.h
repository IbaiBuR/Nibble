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
    QUEEN_PROMO,
    KNIGHT_CAPTURE_PROMO,
    BISHOP_CATURE_PROMO,
    ROOK_CAPTURE_PROMO,
    QUEEN_CAPTURE_PROMO
};

constexpr char promoFlagToChar[] = {
    [KNIGHT_PROMO] = 'n',       [BISHOP_PROMO] = 'b',         [ROOK_PROMO] = 'r',
    [QUEEN_PROMO] = 'q',        [KNIGHT_CAPTURE_PROMO] = 'n', [BISHOP_CATURE_PROMO] = 'b',
    [ROOK_CAPTURE_PROMO] = 'r', [QUEEN_CAPTURE_PROMO] = 'q'};

// clang-format off
// Castling rights update constants
constexpr int castlingRights[SQUARE_NB] = {
    7, 15, 15, 15,  3, 15, 15, 11,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   15, 15, 15, 15, 15, 15, 15, 15,
   13, 15, 15, 15, 12, 15, 15, 14
};
// clang-format on

bool isPromotion(Move move);
bool isCapture(Move move);
bool isEnPassant(Move move);
bool isDoublePush(Move move);
bool isCastling(Move move);

int  makeMove(Move move, Board *board);
void undoMove(Board *board);

char *moveToString(Move move);

Move parseMove(const char *move, const Board *board);
