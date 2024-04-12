#include "search.h"

#include <stdio.h>

#include "bitboard.h"
#include "eval.h"
#include "move.h"
#include "movegen.h"

// Quiescence search, to get rid of the horizon effect
static inline Score
qsearch(Board *board, Score alpha, const Score beta, const int ply, SearchInfo *searchInfo) {
    const Score staticEval = evaluate(board);

    if (staticEval >= beta)
        return beta;
    if (staticEval > alpha)
        alpha = staticEval;

    searchInfo->nodes++;

    MoveList moveList;
    moveList.count = 0;
    generateAllCaptures(board, &moveList, board->stm);

    for (uint32_t i = 0; i < moveList.count; i++)
    {
        if (!makeMove(moveList.moves[i].move, board))
            continue;

        const Score score = -qsearch(board, -beta, -alpha, ply + 1, searchInfo);
        undoMove(board);

        // fail-hard beta-cutoff
        if (score >= beta)
            // node fails high
            return beta;

        if (score > alpha)
        {
            // PV node
            alpha = score;
        }
    }

    return alpha;
}

static inline Score negamax(Board      *board,
                            Score       alpha,
                            const Score beta,
                            const int   depth,
                            const int   ply,
                            SearchInfo *searchInfo) {
    const bool  isRootMove      = ply == 0;
    const bool  inCheck         = bitCount(board->checkers) > 0;
    const Score oldAlpha        = alpha;
    Move        currentBestMove = NOMOVE;
    uint16_t    legalMoveCount  = 0;

    if (depth == 0)
        return qsearch(board, alpha, beta, ply, searchInfo);

    searchInfo->nodes++;

    MoveList moveList;
    generateAllMoves(board, &moveList, board->stm);

    for (uint32_t i = 0; i < moveList.count; i++)
    {
        if (!makeMove(moveList.moves[i].move, board))
            continue;

        legalMoveCount++;
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

    // Checkmate / stalemate detection
    if (!legalMoveCount)
    {
        if (inCheck)
            // Take the shortest available mate
            return -SCORE_MATE + ply;
        else
            // Stalemate
            return 0;
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

    if (searchInfo.bestMove != NOMOVE)
    {
        printf("info score cp %d depth %d nodes %ld\n", score, depth, searchInfo.nodes);
        printf("bestmove %s\n", moveToString(searchInfo.bestMove));
    }
}