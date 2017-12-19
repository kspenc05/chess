//allows movement to occur
#ifndef MOVEMENT_H
#define MOVEMENT_H

//NOTE::
//I've defined move_false as 1, to make validateInput simpler,
//it adds the returns of multiple int functions, which if they fail each return 1,
//in the event none of them fail, they will return 0, which if added is still 0.
#define move_false 1
#define move_true 0

#include "player.h"

//PURPOSE:: checks if a given move is valid or not
//ARGUMENTS:: the board, 
//
//an int array of coords [] where: [0] = prev.X, [1] = prev.Y
//and [2] = future.X, and [3] = future.Y
//
//as well as the two player variables, and a 1 or a 0, abput whether or not to 
//display warnings to the player
//
//RETURNS:: move_true if the move is a legal chess move, otherwise move_false.
//
int isValidMove(char board [8] [9], int coords [4], Player * player, 
    Player * opposing, int messages);

//PURPOSE:: gets user input for a move, and checks if it is legal or not
//
//ARGUMENTS:: four integers to store each X or Y value separately, the board,
//and the two player variables
//
//RETURNS:: returns 0 (move_true) if valid, else 1 (move_false)
//
int validateInput (int * Y1, int * Y2, int * X1, int * X2, 
    char board [8] [9], Player * player, Player * opposing, int messages);

//PURPOSE:: checks if any number can fit within the board's boundaries (between 
//0 and 7).
//
//ARGUMENTS:: the number to check
//
//RETURNS 1 if true, 0 if false
//
int withinBounds(int number);

//PURPOSE:: checks if a particular piece is in danger 
//from the opponent's pieces
//
//ARGUMENTS:: The piece to check, the game board, and the two player variables
//
//RETURNS:: 1 if piece could be attacked, else 0
//
int isInDanger(Piece target, char board [8] [9], 
    Player * opposing, Player * player);

//PURPOSE:: checks if piece is attacking a friendly piece on the board
//
//ARGUMENTS:: the next X and Y position, the player variable, and then a 1 or 0
//to determine wheher to give a warning or not.
//
//RETURNS:: 1 if true, else 0
//
int friendlyFire(int next_X, int next_Y, Player * player, int messages);

//PURPOSE:: checks if knight move is valid
//
//ARGUMENTS:: game board and int coordinate array (X1, Y1, X2, Y2).
//
//RETURNS:: 1 if true (move_true), else 0 (move_false).
//
int valid_Knight_move(int coord [4]); //in check()

//PURPOSE:: gets input from the user
//ARGUMENTS:: each X and Y coordinate to store the move
void input(int * Y1, int * Y2, int * X1, int * X2);

//PURPOSE:: checks if the previous and future moves are valid
// coordinates on the board.
//
//ARGUMENTS:: coordinate array with all four ints to check,
//and a 1 or a 0, to determine whether or not to print warnings
//
//RETURNS:: move_true if they are within bounds
// otherwise returns move_false.
int checkArrayBounds(int coords [4], int messages);

#endif