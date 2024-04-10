#pragma once

#include "types.h"

constexpr int gamePhaseInc[PIECE_NB] = {0, 1, 1, 2, 4, 0, 0, 1, 1, 2, 4, 0};
constexpr int maxGamePhase           = 24;

Score evaluate(const Board *board);