#ifndef DRAW_H

#include "player.h"

#define DRAW_h

#define B_width 39 //how wide the board should print
#define FORWARD 1
#define BACKWARD 0

void title();

void printBoard(char board [8] [9], Player * player, Player * opposing,
    int direction);

#endif