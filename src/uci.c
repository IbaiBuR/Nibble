#include "uci.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "perft.h"

void parsePosition(const char *command, Board *board) {
    command += 9;

    const char *current = command;

    if (strncmp(current, "startpos", 8) == 0)
        parseFen(startPosFen, board);
    else
    {
        current = strstr(command, "fen");
        if (current)
        {
            current += 4;
            parseFen(current, board);
        }
        else
            parseFen(startPosFen, board);
    }

    current = strstr(command, "moves");

    if (current == nullptr)
        return;

    current += 6;
    char strtokBuffer[strlen(current) + 1];
    strcpy(strtokBuffer, current);

    for (const char *currentMove = strtok(strtokBuffer, " "); currentMove;
         currentMove             = strtok(nullptr, " "))
    {
        const Move parsedMove = parseMove(currentMove, board);

        if (parsedMove == NOMOVE)
            break;

        makeMove(parsedMove, board);
    }
}

void parseGo(const char *command, Board *board) {
    command += 3;
    const char *current;

    if ((current = strstr(command, "perft")))
        perftTest(atoi(current + 6), board);
}

void uciLoop() {
    Board board;
    parseFen(startPosFen, &board);

    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);

    while (true)
    {
        char inputBuffer[4096] = {};
        fflush(stdout);

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == nullptr || inputBuffer[0] == '\n')
            continue;

        if (strncmp(inputBuffer, "isready", 7) == 0)
            printf("readyok\n");
        else if (strncmp(inputBuffer, "position", 8) == 0)
            parsePosition(inputBuffer, &board);
        else if (strncmp(inputBuffer, "ucinewgame", 10) == 0)
            parsePosition("position startpos", &board);
        else if (strncmp(inputBuffer, "uci", 3) == 0)
        {
            printf("id name   Nibble %s\n", VERSION);
            printf("id author Ibai Burgos\n");
            printf("uciok\n");
        }
        else if (strncmp(inputBuffer, "go", 2) == 0)
            parseGo(inputBuffer, &board);
        else if (strncmp(inputBuffer, "board", 5) == 0)
            printBoard(board);
        else if (strncmp(inputBuffer, "quit", 4) == 0)
            break;
        else
            printf("Unknown command: %s\n", inputBuffer);
    }
}
