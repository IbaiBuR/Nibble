#pragma once

#include "move.h"
#include "types.h"

constexpr char pieceToChar[PIECE_NB] = "PNBRQKpnbrqk";

constexpr Piece charToPiece[] = {
    ['P'] = W_PAWN,   ['N'] = W_KNIGHT, ['B'] = W_BISHOP, ['R'] = W_ROOK,
    ['Q'] = W_QUEEN,  ['K'] = W_KING,   ['p'] = B_PAWN,   ['n'] = B_KNIGHT,
    ['b'] = B_BISHOP, ['r'] = B_ROOK,   ['q'] = B_QUEEN,  ['k'] = B_KING};

constexpr Color pieceColor[] = {
    [W_PAWN] = WHITE,   [B_PAWN] = BLACK,   [W_KNIGHT] = WHITE, [B_KNIGHT] = BLACK,
    [W_BISHOP] = WHITE, [B_BISHOP] = BLACK, [W_ROOK] = WHITE,   [B_ROOK] = BLACK,
    [W_QUEEN] = WHITE,  [B_QUEEN] = BLACK,  [W_KING] = WHITE,   [B_KING] = BLACK};

constexpr Piece promotionToPiece[] = {
    [KNIGHT_PROMO] = KNIGHT, [BISHOP_PROMO] = BISHOP, [ROOK_PROMO] = ROOK, [QUEEN_PROMO] = QUEEN};

void movePiece(Board *board, Piece piece, Square from, Square to);

Piece pieceOnSquare(const Board *board, Square square);