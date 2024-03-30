#include "perft.h"

#include <stdio.h>

#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "movegen.h"

uint64_t nodes;

void perftDriver(const int depth, Board *board) {
    MoveList moveList;

    if (depth == 0)
    {
        nodes++;
        return;
    }

    generateAllMoves(board, &moveList, board->stm);

    for (uint32_t i = 0; i < moveList.count; i++)
    {

        if (!makeMove(moveList.moves[i].move, board))
            continue;

        perftDriver(depth - 1, board);

        undoMove(board);
    }

    printf("nodes: %ld\n", nodes);
}