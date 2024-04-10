#include "bitboard.h"
#include "psqt.h"
#include "uci.h"

int main() {
    initBB();
    initPSQT();
    uciLoop();
    return 0;
}
