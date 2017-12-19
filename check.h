//This header is for the source file I have made containing all of the 
//additional checks required by the Check() in boardFcns.c used to determine
//each game condition

#ifndef CHECK_H
#define CHECK_H

#include "movement.h"
#include "directions.h"

//number of possible checkable directions
#define num_direcns 8

//the necessary game states
enum victories
{
    king_is_safe,
    check,
    check_mate,
    stale_mate
};

//PURPOSE:: sets up the X and Y positions of the pieces around the king
//
//ARGUMENTS:: the King's piece and the array of pieces around the King (at least
//8 in order for this function to work).)
void set_aroundKing(Piece King, Piece aroundKing [num_direcns]);

//PURPOSE:: determines if the player is in check or in checkmate
//
//ARGUMENTS:: the board, both player variables and the King
//
//RETURNS:: check or checkmate
int CheckMate (char board [8] [9], Player * player, Player * opposing, Piece King);

//PURPOSE:: determines if the Kings can attack eacth other currently
//
//ARGUMENTS:: the two player variables, game board
//
//RETURNS:: 0 or 1
int kings_are_2_close (Player * player, Player * opposing, char board [8] [9]);

//PURPOSE:: determines if checkmate is possible, by ensuring there are
//enough pieces for checkmate.
//ARGUMENTS:: the board and both player variables.
//RETURNS:: 1 if there is sufficient pieces for either player to achieve
//checkmate, otherwise 0.
int Enough_pieces(char board [8] [9], Player * player, Player * opposing);

//PURPOSE:: checks if the King can move or not
//
//ARGUMENTS:: the array of pieces around the king, the board, and the two player
//variables.
//RETURNS:: 1 if the king can move, 0 if it cannot move
//
int can_KingMove (Piece aroundKing[num_direcns], char board [8] [9],
    Player * player, Player * opposing);

//PURPOSE:: checks if the king is in stalemate, or is safe
//
//ARGUMENTS:: the board, and the two player variables
//
//RETURNS stalemate, if the king is in stalemate OR
//king_is_safe, if the king is not in stalemate.
int Stalemate(char board [8] [9], Player * player, Player * opposing);

#endif