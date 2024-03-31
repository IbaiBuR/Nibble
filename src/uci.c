#include "uci.h"

#include <string.h>

#include "board.h"
#include "move.h"

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
