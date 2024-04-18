#include "search.h"

#include <stdio.h>
#include <string.h>

#include "bitboard.h"
#include "eval.h"
#include "move.h"
#include "movegen.h"
#include "movesort.h"
#include "piece.h"
#include "util.h"

void copyPV(SearchData *searchData, const int ply) {
    for (int i = ply + 1; i < searchData->pv.pvLength[ply + 1]; i++)
        searchData->pv.moves[ply][i] = searchData->pv.moves[ply + 1][i];
}

void printPV(const SearchData *searchData) {
    for (int i = 0; i < searchData->pv.pvLength[0]; i++)
        printf("%s ", moveToString(searchData->pv.moves[0][i]));
    printf("\n");
}

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

        // Fail-hard beta-cutoff
        if (score >= beta)
            // Node fails high
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
    const bool inCheck           = bitCount(board->checkers) > 0;
    uint16_t   legalMoveCount    = 0;
    searchData->pv.pvLength[ply] = ply;

    if (depth == 0)
        return qsearch(board, alpha, beta, ply, searchInfo, searchData);

    // Return static eval if we are too deep to avoid overflows
    if (ply >= MAX_PLY)
        return evaluate(board);

    searchInfo->nodes++;

    // Check extension (https://www.chessprogramming.org/Check_Extensions)
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

        // Fail-hard beta-cutoff
        if (score >= beta)
        {
            if (isQuiet(currentMove))
            {
                // Killer moves
                searchData->killers[1][ply] = searchData->killers[0][ply];
                searchData->killers[0][ply] = currentMove;
            }
            // Node fails high
            return beta;
        }

        if (score > alpha)
        {
            if (isQuiet(currentMove))
                // History moves
                searchData->history[pieceOnSquare(board, from(currentMove))][to(currentMove)] +=
                    depth;
            // PV node
            alpha = score;

            // Add moves to the PV (principal variation)
            searchData->pv.moves[ply][ply] = currentMove;
            copyPV(searchData, ply);
            searchData->pv.pvLength[ply] = searchData->pv.pvLength[ply + 1];
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

    // Node fails low
    return alpha;
}

void initSearch(SearchInfo *info, SearchData *searchData) {
    info->nodes = 0;
    memset(searchData->killers, NOMOVE, sizeof(searchData->killers));
    memset(searchData->history, 0, sizeof(searchData->history));
    memset(searchData->pv.moves, NOMOVE, sizeof(searchData->pv.moves));
    memset(searchData->pv.pvLength, 0, sizeof(searchData->pv.pvLength));
}

void search(Board *board, const int depth) {
    SearchInfo searchInfo;
    SearchData searchData;
    initSearch(&searchInfo, &searchData);

    // Iterative deepening loop
    for (int currentDepth = 1; currentDepth <= depth; currentDepth++)
    {
        const Score score   = negamax(board, -SCORE_INFINITE, SCORE_INFINITE, currentDepth, 0,
                                      &searchInfo, &searchData);
        searchInfo.bestMove = searchData.pv.moves[0][0];

        printf("info score cp %d depth %d nodes %" PRIu64 " pv ", score, currentDepth,
               searchInfo.nodes);
        printPV(&searchData);
    }
    printf("bestmove %s\n", moveToString(searchInfo.bestMove));
}