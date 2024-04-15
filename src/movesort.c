#include "movesort.h"

#include <stdlib.h>

#include "move.h"

static inline int compareMoves(const void *a, const void *b) {
    const ScoredMove *mv1 = a;
    const ScoredMove *mv2 = b;

    if (mv1->score < mv2->score)
        return 1;
    if (mv1->score > mv2->score)
        return -1;
    return 0;
}

inline void sortMoves(const Board *board, MoveList *moveList) {
    scoreAllMoves(board, moveList);
    qsort(moveList->moves, moveList->count, sizeof(ScoredMove), compareMoves);
}
