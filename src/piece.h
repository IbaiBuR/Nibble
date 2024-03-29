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

constexpr PieceType promotionToPieceType[] = {
    [KNIGHT_PROMO] = KNIGHT,     [BISHOP_PROMO] = BISHOP,         [ROOK_PROMO] = ROOK,
    [QUEEN_PROMO] = QUEEN,       [KNIGHT_CAPTURE_PROMO] = KNIGHT, [BISHOP_CATURE_PROMO] = BISHOP,
    [ROOK_CAPTURE_PROMO] = ROOK, [QUEEN_CAPTURE_PROMO] = QUEEN};

constexpr PieceType pieceToPieceType[] = {
    [W_PAWN] = PAWN,     [B_PAWN] = PAWN,     [W_KNIGHT] = KNIGHT, [B_KNIGHT] = KNIGHT,
    [W_BISHOP] = BISHOP, [B_BISHOP] = BISHOP, [W_ROOK] = ROOK,     [B_ROOK] = ROOK,
    [W_QUEEN] = QUEEN,   [B_QUEEN] = QUEEN,   [W_KING] = KING,     [B_KING] = KING};

void removePiece(Board *board, Piece piece, Square square);
void addPiece(Board *board, Piece piece, Square square);
void movePiece(Board *board, Piece piece, Square from, Square to);

Piece pieceOnSquare(const Board *board, Square square);