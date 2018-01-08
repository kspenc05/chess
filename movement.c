#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "movement.h"

//PURPOSE:: converts coordinate letter into a number value
//ARGUMENTS:: letter to get value of
//NOTE:: is case insenstive
//
//RETURNS:: the converted ASCII into a proper number
int convertToInt(int boardChar)
{
    return toupper(boardChar) - 'A';
}

//PURPOSE::: checks if the player has entered a move or not.
//ARGUMENTS:: both X and Y coordinate pairs.
//RETURNS:: move_true if valid, else move_false.
int isEntered(int Y1, int Y2, int X1, int X2)
{
    if(Y1 == -99 || Y2 == -99 || X1 ==  ' ' || X2 ==  ' ')
    {
        printf("You have incorrectly entered a move.\n"
            "Please enter two coordinate pairs, where both pairs\n"
            "are positions on the board\n\neg. \"D2 D3\"\n\n"
            "Please enter a move within that format\n\n"
        );
        return move_false;
    }
    return move_true;
}

int withinBounds(int number)
{
    return (number < 0 || number > 7) ? 1 : 0;
}

//PURPOSE:: checks if two numbers are 1 away from each other, eg. 2 and 3 are,
// but also 2 and 1 are.
// ARGUMENTS:: the two numbers to check
//RETURNS:: 1 if the numbers have a difference of 1 or -1, 0 otherwise.
int off_by(int diff, int a, int b)
{
    return (a - b == diff || a - b == diff * -1) ? 1 : 0;
}

int checkArrayBounds(int coords[4], int messages)
{
    for(int i = 0; i < 4; i++)
    {
        if(withinBounds(coords[i])) //if the move is outside of the board
        {
            if(messages)
            {
                printf("That position is outside the boundaries of the board.\n"
                    "Enter another move.\n\n");
            }
            return move_false;
        }
    }
    return move_true;
}

//PURPOSE:: checks if a two-move pawn move is legal or not
//
//ARGUMENTS:: the player variable, and the int coordinate array 
int valid_2Square_move(Player * current, int coords [4], char type)
{
    Piece * piece = findPiece(current, coords[0], coords[1]);
        
    return (piece != NULL && piece->moved == 0 && type == '_') ?
        move_true : move_false;
}

//PURPOSE:: checks it the pawn can be promoted or not, after a move 
//has been shown to be legal
//
//ARGUMENTS:: the player number, the int coordinate array, and the board
void Promotion(int player_num, int coords [4], char * promotedUnit)
{
    if((player_num == 1 && coords[3] == 7) || 
        (player_num == 2 && coords[3] == 0))
    {
        char string [10];
        int match;
        
        printf("Your pawn has advanced to the end of the board\n"
            "What would you like to promote it to?\n\n"
            "Options: ['Q', 'R', 'B', 'N']\n\n"
        );
        
        do
        {
            fgets(string, 10, stdin); //in case user types more by accident
            string[strlen(string) - 1] = '\0';
        
            string[0] = toupper(string[0]);
            
            match = -1;
            
            switch(string[0])
            {
                case 'Q': case 'B': case 'R': case 'N': match = 1; break;
                default: printf("invalid promotion\n\n"); break;
            }
        } while(match != 1);
        
        *promotedUnit = string[0];
    }
}

//PURPOSE:: checks if En Passente can be performed or not by current player
//
//ARGUMENTS:: the board, the coordinate array, the current player number 
//and the opposing player variable.
//
//RETURNS:: move_true if legal, else move_false.
int En_Passente(char board[8] [9], int coords[4], int player_num, 
    Player * enemy)
{
    Piece pawn = enemy->lastMove[1];
    
    //checks if the last move one where there is a pawn and that the opposing 
    //player moved that pawn by 2 in either direction
    if( off_by(2, pawn.Y, enemy->lastMove[0].Y) &&
        off_by(1, coords[0] + 1, pawn.X)        &&
        board[ pawn.Y ] [ pawn.X ] == 'P'       &&
        coords[1] == pawn.Y)
    {
        if( ((coords[3] - 1 == pawn.Y && player_num == 1) ||
            (coords[3] + 1 == pawn.Y && player_num == 2)) &&
            coords[2] == pawn.X) 
        {
            if(board[ coords[3] ] [ coords[2] ] == '_')
            {
                Piece * piece = findPiece(enemy, pawn.X, pawn.Y);
                
                if(piece != NULL)
                {
                    remove_piece(board, piece);
                    return move_true;
                }
            }
        }
    }
    return move_false;
}

//PURPOSE:: checks if valid pawn move
//
//ARGUMENTS:: the board, the coordinate array, and both player variables
//
//RETURNS:: 1 if invalid move, 0 if valid
//
int valid_Pawn_move(char board[8] [9], int coords[4], Player * current, Player * enemy)
{
    int status = move_false;
    
    if(En_Passente(board, coords, current->num, enemy) == move_true)
        return move_true;
    
    int diffY = coords[3] - coords[1];
    int diffX = coords[2] - coords[0];
    
    //check if it is a forward move, and then if 1 or 2 square move
    if(board[ coords[3]] [ coords[2]] == '_' && diffX == 0)
    {
        if(current->num == 1)
        {
            switch(diffY)
            {
                case 1: status = move_true; break;
                case 2: status = valid_2Square_move(current, coords, 
                    board[ coords[3] - 1] [ coords[2]]); break;
            }
        }
        else if(current->num == 2)
        {
            switch(diffY)
            {
                case -1: status = move_true; break;
                case -2: status = valid_2Square_move(current, coords,
                            board[ coords[3] + 1] [ coords[2]]); break;
            }
        }
    }
    
    //check if diagonal move, 
    //attackable square cannot be a space
    else if(board[coords[3]] [coords[2]] != '_' &&
        (diffX == -1 || diffX == 1) )
    {
        if( (diffY == 1 && current->num == 1) ||
            (diffY == -1 && current->num == 2) )
        {
            status = move_true;
        }
    }
    
    //if the move was valid, does the pawn need to be promoted?
    if(status == move_true)
    {
        Promotion(current->num, coords, &board[ coords[1]] [ coords[0]] );
    }
    return status;
}

//PURPOSE:: determines the direction to increment based on direction
//
//ARGUMENTS:: the two numbers to compare
//
int setDir(int comp1, int comp2)
{
    return (comp1 < comp2) ? 1 : -1;
}

//PURPOSE:: sets loop conditions for rook
//
//ARGUMENTS:: the two numbers to compare,
//the amount to increment, and the start and end conditions
//
void setLoopConds_Rook(int comp1, int comp2, 
    int * incr, int * start, int * end)
{
    *incr = setDir(comp1, comp2);
    *start = comp1;
    *end = comp2;
}

//PURPOSE:: checks if valid rook move
//
//ARGUMENTS:: board, coordinate array
//
//RETURNS 1 if invalid, 0 if valid
//
int valid_Rook_move(char board[8] [9], int coords[4])
{
    int incr, endCond, vertical, i;
    
    if(coords[0] - coords[2] == 0) //if vertical move
    {
        setLoopConds_Rook(coords[1], coords[3], &incr, &i, &endCond);
        vertical = 1;
    }
    else if(coords[1] - coords[3] == 0) //if horizontal move
    {
        setLoopConds_Rook(coords[0], coords[2], &incr, &i, &endCond);
        vertical = 0;
    }
    else
    {
        return move_false;
    }
    
    //check each square to ensure there are no obstacles in the rook's way
    while( (i = i + incr) != endCond) 
    {
        if(vertical == 0 && board[coords[1] ] [ i ] != '_')
            return move_false;
            
        else if(vertical == 1 && board[ i ] [ coords[0] ] != '_')
            return move_false;
    }
    return move_true;
}

//PURPOSE:: checks if bishop move is valid
//
//ARGUMENTS:: board, and coordinate array
//
//RETURNS:: 1 if invalid, else 0 if valid move
int valid_Bishop_move(char board[8] [9], int coords [4])
{
    int diffX = coords[0] - coords[2];
    int diffY = coords[1] - coords[3];
    
    if(coords[0] == coords[2] || coords[1] == coords[3] ||
        (diffX != diffY && diffX * -1 != diffY)) //if the move is not a diagonal
    {
        return move_false;
    }
    
    int incrI = setDir(coords[1], coords[3]);
    int incrJ = setDir(coords[0], coords[2]);
    int j = incrJ + coords[0];
    
    //just checks X coord, because the X and Y are both incremented each time
    for(int i = coords[1] + incrI; i != coords[3]; i = i + incrI)
    {
        if(board[i] [j] != '_')
        {
            return move_false;
        }
        j = j + incrJ;
    }
    return move_true;
}

//PURPOSE:: checks if the set of coords is an L-shape one way, 
// checks if the X's have a diff of 1, and the Y's have a diff of 2.
//ARGUMENTS:: the two X and two Y coordinates to check.
//RETURNS:: 1 if it is a proper L-shape move, 0 if otherwise.
int L_shape (int X1, int X2, int Y1, int Y2)
{
    return (off_by(1, X2, X1) && off_by(2, Y2, Y1)) ? 1 : 0;
}

//PURPOSE:: determines if it is a valid knight move
//ARGUMENTS:: the coordinate array
//
//RETURNS:: move_true if valid, else move_false.
int valid_Knight_move(int coords [4])
{
    return (L_shape(coords[0], coords[2], coords[1], coords[3]) == 1 ||
        L_shape(coords[1], coords[3], coords[0], coords[2]) == 1) ? 
        move_true : move_false;
}

//PURPOSE:: determines if valid king move or not
//
//ARGUMENTS:: the coordinate array
//
//RETURNS:: move_true if a valid move, else move_false.
int valid_King_move(int coords[4])
{
    return (off_by(1, coords[3], coords[1]) || off_by(1, coords[2], coords[0])) ? 
        move_true : move_false;
}

//PURPOSE:: checks if the piece exists or not
//ARGUMENTS:: the starting X and Y positions, and the player variable.
//
//RETURNS:: move_true if valid move, else move_false.
int doesPieceExist(int start_X, int start_Y, Player * current, int messages)
{
    if(findPiece(current, start_X, start_Y))  
        return move_true;
    
    if(messages)
        printf("Could not find a piece at that location\n");
    return move_false;
}

//PURPOSE:: checks if the future position belongs to the current player
//(are they attacking a piece they already own)
//
//ARGUMENTS:: future X pos, future Y pos, and the player variable
//
//RETURNS:: move_true if valid move, else move_false.
int friendlyFire(int next_X, int next_Y, Player * current, int messages)
{
    if(findPiece(current, next_X, next_Y))  
    {
        if(messages)
            printf("That piece already belongs to you\nYou cannot take it\n");
        return move_false;
    }
    return move_true;
}

int isValidMove(char board[8] [9], int coords [4], 
    Player * current, Player * enemy, int messages)
{
    int status;
    
    //added because of invalid read of size 1, without this, will access invalid
    //piece location at the switch
    if(isEntered(coords[1], coords[3], coords[2], coords[0]) == move_false ||
        doesPieceExist(coords[0], coords[1], current, messages) == move_false  ||
        checkArrayBounds(coords, messages) == move_false)
    {
        return move_false;
    }
    
    switch(board[ coords[1] ] [ coords[0] ]) //Y, then X position
    {
        case 'B': status = valid_Bishop_move(board, coords); break;
        case 'K': status = valid_King_move(coords); break;
        case 'N': status = valid_Knight_move(coords); break;
        
        case 'P': 
        {
            status = valid_Pawn_move(board, coords, current, enemy); 
            break;
        }
        case 'Q': 
        {
            status = (valid_Rook_move(board, coords) == 0 ||
                valid_Bishop_move(board, coords) == 0) ? move_true : move_false; 
            break;
        }
        case 'R': status = valid_Rook_move(board, coords); break;
        default: return move_false;
    }
    if(status == move_false && messages)
    {
        printf("that move is not possible for that piece\n");
    }
    return status;
}

int isInDanger(Piece target, char board[8] [9], Player * enemy, Player * current)
{
    for(int i = 0; i < NUM_PIECES; i++)
    {
        Piece * piece = &enemy->all[i];
        if(piece->Y != -1 && //if the Y is -1, so is the X 
            board[ piece->Y ] [ piece->X ] != 'K')
        {
            int coords [4] = {piece->X, piece->Y, target.X, target.Y};
                
            if(isValidMove(board, coords, enemy, current, 0) == move_true)
            {
                return 1;
            }
        }
    }
    return 0;
}

//PURPOSE:: checks if the castling is valid
//
//ARGUMENTS:: current direction, the board, and the two player variables.
//
//RETURNS:: move_true if valid, otherwise move_false.
int isValid_Castling (int * mode, char board[8] [9], Player * current, Player * enemy)
{
    int rook_X, incr;
    
    switch(*mode)
    {
        case 'L': 
        {
            rook_X = 0; 
            incr = -1; 
            break;
        }
        case 'R': 
        {
            rook_X = 7; 
            incr = 1; 
            break;
        }
    }
    
    Piece * King = findKing(current, board);
    Piece * Rook = findPiece(current, rook_X, King->Y);
    
    if(Rook == NULL || Rook->moved || King->moved)
    {
        return move_false;
    }
    
    for(int i = King->X; i != rook_X; i = i + incr)
    {
        if(board[King->Y] [i] != '_' && i != King->X)
        {
            return move_false;
        }
        
        Piece targetSQ = {i, King->Y, 0};
        
        if(isInDanger(targetSQ, board, enemy, current) == 1)
            return move_false;
    }
    return move_true;
}

//PURPOSE:: generic swap function for ints
//ARGUMENTS:: the two ints to swap
void Swap (int * a, int * b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

void input(int * Y1, int * Y2, int * X1, int * X2)
{
    char buffer [20];
    *Y1 = *Y2 = -99;
    *X1 = *X2 = ' ';
    
    fgets(buffer, 50, stdin);
    buffer[strlen(buffer) - 1] = '\0';
    sscanf(buffer, "%c%d %c%d", (char *) X1, Y1, (char *) X2, Y2);
    
    //if the user enters both coords in this format: "6c", instead of "C6"
    //swap them both
    switch(isalpha(*Y1) + isalpha(*Y2))
    {
        case !(0): 
        {
            Swap(X1, Y1); 
            Swap(X2, Y2); 
            break;
        }
    }
}

int validateInput (int * Y1, int * Y2, int * X1, int * X2, 
    char board[8] [9], Player * current, Player * enemy, int messages)
{
    if(*X1 == 'L' || *X1 == 'R')
    {
        return isValid_Castling(X1, board, current, enemy);
    }
    
    *X1 = convertToInt(*X1);
    *X2 = convertToInt(*X2);
    
    int coords [4] = { *X1, --(*Y1), *X2, --(*Y2) };
    
    return friendlyFire(coords[2], coords[3], current, messages) + 
        isValidMove(board, coords, current, enemy, messages);
}