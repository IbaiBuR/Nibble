#include "search.h"

#include <stdio.h>
#include <string.h>

#include "bitboard.h"
#include "eval.h"
#include "move.h"
#include "movegen.h"
#include "movesort.h"
#include "piece.h"

// Quiescence search, to get rid of the horizon effect
static inline Score qsearch(Board      *board,
                            Score       alpha,
                            const Score beta,
                            const int   ply,
                            SearchInfo *searchInfo,
                            SearchData *searchData) {
    const Score staticEval = evaluate(board);

    if (staticEval >= beta)
        return beta;
    if (staticEval > alpha)
        alpha = staticEval;

    searchInfo->nodes++;

    MoveList moveList;
    moveList.count = 0;
    generateAllCaptures(board, &moveList, board->stm);
    sortMoves(board, &moveList, ply, searchData);

    for (uint32_t i = 0; i < moveList.count; i++)
    {
        if (!makeMove(moveList.moves[i].move, board))
            continue;

        const Score score = -qsearch(board, -beta, -alpha, ply + 1, searchInfo, searchData);
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
                            int         depth,
                            const int   ply,
                            SearchInfo *searchInfo,
                            SearchData *searchData) {
    const bool  isRootMove      = ply == 0;
    const bool  inCheck         = bitCount(board->checkers) > 0;
    const Score oldAlpha        = alpha;
    Move        currentBestMove = NOMOVE;
    uint16_t    legalMoveCount  = 0;

    if (depth == 0)
        return qsearch(board, alpha, beta, ply, searchInfo, searchData);

    searchInfo->nodes++;

    // Check extensions (https://www.chessprogramming.org/Check_Extensions)
    if (inCheck)
        depth += 1;

    MoveList moveList;
    generateAllMoves(board, &moveList, board->stm);
    sortMoves(board, &moveList, ply, searchData);

    for (uint32_t i = 0; i < moveList.count; i++)
    {
        const Move currentMove = moveList.moves[i].move;

        if (!makeMove(currentMove, board))
            continue;

        legalMoveCount++;
        const Score score =
            -negamax(board, -beta, -alpha, depth - 1, ply + 1, searchInfo, searchData);
        undoMove(board);

        // fail-hard beta-cutoff
        if (score >= beta)
        {
            if (isQuiet(currentMove))
            {
                // killer moves
                searchData->killers[1][ply] = searchData->killers[0][ply];
                searchData->killers[0][ply] = currentMove;
            }
            // node fails high
            return beta;
        }

        if (score > alpha)
        {
            if (isQuiet(currentMove))
                // history moves
                searchData->history[pieceOnSquare(board, from(currentMove))][to(currentMove)] +=
                    depth;
            // PV node
            alpha = score;

            if (isRootMove)
                currentBestMove = currentMove;
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

void initSearch(SearchInfo *info, SearchData *searchData) {
    info->nodes    = 0;
    info->bestMove = NOMOVE;
    memset(searchData->killers, NOMOVE, sizeof(searchData->killers));
    memset(searchData->history, 0, sizeof(searchData->history));
}

void search(Board *board, const int depth) {
    SearchInfo searchInfo;
    SearchData searchData;
    initSearch(&searchInfo, &searchData);

    const Score score =
        negamax(board, -SCORE_INFINITE, SCORE_INFINITE, depth, 0, &searchInfo, &searchData);

    if (searchInfo.bestMove != NOMOVE)
    {
        printf("info score cp %d depth %d nodes %ld\n", score, depth, searchInfo.nodes);
        printf("bestmove %s\n", moveToString(searchInfo.bestMove));
    }
}