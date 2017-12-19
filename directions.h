//header for the source file designed to deal with the additional checks 
//required for Checkmate() used in check.c, when determining if blocking
//is possible, or if there are too many directions for it to be done.

#ifndef DIRECS_H
#define DIRECS_H

#include "movement.h"

enum direction
{
    N,
    S,
    E,
    W,
    NW,
    NE,
    SW,
    SE
};

//PURPOSE:: finds out whether the bishop or the Rook is a danger to the king,
//depending on the direction it is checking.
//
//ARGUMENTS:: the current direction it is checking
char setDanger(int * mode);

//PURPOSE:: finds which directions the King is currently in check (if any)
//
//ARGUMENTS:: the current direction, where the King is located, the board,
//and the current player direction.
void findCheckDirns(int mode, Piece King, char board [8] [9], Player * player);

//PURPOSE:: gets the first direction that is in danger, as marked in the int 
//array by a 1 instead of 0
//
//ARGUMENTS:: the array of directions to check
//
//RETURNS:: the current direction that is in danger
int getDir(int * directions);

//PURPOSE:: resets all directions, and the number of directions
//so that all directions can be checked again
//
//ARGUMENTS:: the array of directions, and the number of directions
void resetDir(int * directions, int * numDir);

//PURPOSE:: sets the X and Y increments based on the current direction.
//and the which player is the current player
//
//ARGUMENTS:: the direction, the Y incremenet, the X increment, and the player
//number.
void setIncr_XandY(int * dir, int * incrY, int * incrX, int player_num);

//PURPOSE:: returns the determined end condition, based on the increment
//
//ARGUMENTS:: the increment (doesn't matter if X or Y).
int get_endCond(int * incr);

#endif