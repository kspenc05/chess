//header for boardFunctions.c, a file used to do the three functions required 
//of the game board, setting each character, moving pieces around, and seeing
//if the game has been won, by determing if check, checkmate or stalemate 
//have occured using the check(), which itself calls upon "check.h".

#ifndef BOARDFUNCTIONS_H
#define BOARDFUNCTIONS_H

#include "check.h"
#include "player.h"

//PURPOSE:: puts all the pieces on the board
//ARGUMENTS:: the board
void setBoard(char board [8] [9]);

//PURPOSE:: moves any piece on the board
//ARGUMENTS:: the board, the player being moved, and the other player's number 
//of pieces.
void Move(char board[8] [9], Player * current, Player * enemy, int get_input);

//PURPOSE:: see if the current player is in check or not
//ARGUMENTS:: the board, and the two player variables
int Check (char board [8] [9], Player * current, Player * enemy);

#endif