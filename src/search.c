#include "search.h"

#include "board.h"
#include "eval.h"
#include "move.h"
#include "movegen.h"

#include <stdio.h>

Score negamax(Board      *board,
              Score       alpha,
              const Score beta,
              const int   depth,
              const int   ply,
              SearchInfo *searchInfo) {
    const bool  isRootMove      = ply == 0;
    Move        currentBestMove = NOMOVE;
    const Score oldAlpha        = alpha;

    if (depth == 0)
        return evaluate(board);

    searchInfo->nodes++;

    MoveList moveList;
    generateAllMoves(board, &moveList, board->stm);

    for (uint32_t i = 0; i < moveList.count; i++)
    {
        copyBoardState(board);

        if (!makeMove(moveList.moves[i].move, board))
            continue;

        const Score score = -negamax(board, -beta, -alpha, depth - 1, ply + 1, searchInfo);
        undoMove(board);

        // fail-hard beta-cutoff
        if (score >= beta)
            // node fails high
            return beta;

        if (score > alpha)
        {
            // PV node
            alpha = score;

            if (isRootMove)
                currentBestMove = moveList.moves[i].move;
        }
    }

    if (oldAlpha != alpha)
        searchInfo->bestMove = currentBestMove;
    // node fails low
    return alpha;
}

void search(Board *board, const int depth) {
    SearchInfo searchInfo;
    searchInfo.nodes    = 0;
    searchInfo.bestMove = NOMOVE;
    const Score score   = negamax(board, -SCORE_INFINITE, SCORE_INFINITE, depth, 0, &searchInfo);

    printf("bestmove %s score %d\n", moveToString(searchInfo.bestMove), score);
}