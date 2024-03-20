#include "move.h"

#include <stdio.h>

#include "board.h"

char *moveToString(const Move move) {
    static char buffer[6];

    sprintf(buffer, "%s%s", squareToCoordinates[from(move)], squareToCoordinates[to(move)]);

    return buffer;
}