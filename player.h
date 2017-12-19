#ifndef PLAYER_H
#define PLAYER_H

#define NUM_PIECES 16 //the default number of pieces for each player

typedef struct Piece
{
    short int X;
    short int Y;
    short int moved; //important for castling, en passente, and two-square pawn moves
} Piece;

typedef struct Player //stores all required information about the player
{
    Piece all [16]; //stores the Piece of each piece owned by the player
    int num; //player number 1 or 2?
    
    int Danger [8]; //marks off all directions the player is in check.
    int numDir; //stores number of directions king is in danger
    
    Piece lastMove [2]; //stores last two position changes. to store the last move
} Player;

//PURPOSE:: finds where the player's king is located
//
//ARGUMENTS:: the current player, and the board
//
//RETURNS:: the current player's King, if it can't be found, returns NULL
Piece * findKing(Player * player, char board [8] [9]);

//PURPOSE:: default constructor of the Piece type:
//will set the X, Y, and moved values.
//
//ARGUMENTS:: The Piece, as well as the X, the Y, and moved values.
void set_Piece(Piece * piece, int X, int Y, int moved);

//PURPOSE:: gets the X and Y values of a single piece.
//
//ARGUMENTS:: the current piece, and two integers
//to store the X and Y values.
void get_XandY(Piece piece, int * X, int * Y);

//PURPOSE:: finds the position in the current player's piece array of where
//the requested piece is located.
//
//ARGUMENTS:: the player variables, and the X and Y (which are used to search
//for the requested piece).
//
//RETURNS:: if sucessful, returns the piece, else returns NULL
Piece * findPiece(Player * player, int X, int Y);

//ARGUMENTS:: the board and the coordinates of the piece
//RETURNS:: returns what the character on the board is
char piece_type(char board [8] [9], int X, int pos);

//PURPOSE:: removes a piece from the other player's Piece array
//
//ARGUMENTS:: the opposing player, the board, and the position where 
//the Piece to remove is located.
void remove_piece(char board [8] [9], Piece * piece);

//PURPOSE:: initializes both player variables
//
//ARGUMENTS:: both player variables.
void initPlayers(Player * player1, Player * player2);

#endif