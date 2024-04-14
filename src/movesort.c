#include "movesort.h"

#include "move.h"

void swapMoves(ScoredMove *mv1, ScoredMove *mv2) {
    const ScoredMove aux = *mv1;
    *mv1                 = *mv2;
    *mv2                 = aux;
}

inline void sortMoves(const Board *board, MoveList *moveList) {
    scoreAllMoves(board, moveList);

    // Selection sort (https://en.wikipedia.org/wiki/Selection_sort)
    for (int i = 0; i < moveList->count - 1; i++)
    {
        int jMax = i;

        for (int j = i + 1; j < moveList->count; j++)
        {
            if (moveList->moves[j].score > moveList->moves[jMax].score)
                jMax = j;
        }

        if (jMax != i)
            swapMoves(&moveList->moves[i], &moveList->moves[jMax]);
    }
}
