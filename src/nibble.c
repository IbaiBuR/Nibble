#include <stdio.h>

#include "bitboard.h"
#include "board.h"
#include "types.h"

int main() {
    initBB();
    Board board;
    initializeBoard(&board);
    parseFen(startPosFen, &board);
    printBoard(board);
    return 0;
}
