#pragma once

#include "types.h"

constexpr Score SCORE_INFINITE = 32000;
constexpr Score SCORE_MATE     = 31500;

void search(Board *board, int depth);