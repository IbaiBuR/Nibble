#include "perft.h"

#include <stdio.h>

#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "util.h"

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
}

uint64_t perftTest(const int depth, Board *board) {
    nodes = 0ULL;
    MoveList moveList;

    printf("\n Running performance test...\n\n");

    generateAllMoves(board, &moveList, board->stm);

    const long startTime = getTimeMS();

    for (uint32_t i = 0; i < moveList.count; i++)
    {
        const Move currentMove = moveList.moves[i].move;

        if (!makeMove(currentMove, board))
            continue;

        const uint64_t cumulatedNodes = nodes;

        perftDriver(depth - 1, board);

        undoMove(board);

        const uint64_t old = nodes - cumulatedNodes;

        printf(" %s: %" PRIu64 "\n", moveToString(currentMove), old);
    }

    const long endTime = getTimeMS();

    printf("\n");
    printf(" Depth: %d\n", depth);
    printf(" Nodes: %" PRIu64 "\n", nodes);
    printf(" Time : %ldms\n", endTime - startTime);
    printf(" NPS  : %" PRIu64 "\n\n", nodes / MAX(1, (endTime - startTime)) * 1000);

    return nodes;
}