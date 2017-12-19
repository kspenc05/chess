#include "directions.h"
#include <stdio.h>
#include <string.h>

char setDanger(int * mode)
{
    switch(*mode)
    {
        case NE: case SE: case NW: case SW: return 'B';
        default: return 'R'; 
    }
}

//PURPOSE:: checks if a square on the board can attack 
// horizontally or vertically
//
//ARGUMENTS:: the character representation, the X and Y, the player variable,
// the number of previously found dangerous pieces, and the secondary danger
// 'B' or 'R': since 'Q' is always a danger in vertical or diagonal attacks.
//
//returns 1 if there is danger, otherwise 0.
//
int markIfDanger (char piece, int X, int Y, Player * player,
    int * numPieces, char dangerPiece)
{
    if(piece == 'Q'|| 
        piece == dangerPiece)
    {
        if(findPiece(player, X, Y) == NULL && //if it's not player's piece
            *numPieces == 0)
        {    
            player->numDir++;
            return 1;
        }
    }
    else if(piece != '_')
    {
        (*numPieces)++;
    }
    return 0;
}

void findCheckDirns(int mode, Piece King, char board [8] [9], Player * player)
{
    int numPieces = 0, i, j, incr1, incr2, end1, end2;
    
    char danger = setDanger(&mode);
    setIncr_XandY(&mode, &incr1, &incr2, player->num);
    
    end1 = get_endCond(&incr1);
    end2 = get_endCond(&incr2);
    
    
    i = King.Y;
    j = King.X;

    while(i != end1 && j != end2 && 
        player->Danger[mode] == 0 && numPieces == 0)
    {
        j = j + incr2;
        i = i + incr1;
        
        if(withinBounds(i) || withinBounds(j))  
            break;

        //if it equals 1, will set it to 1, otherwise, 
            //it does nothing, will set to 0 again
        player->Danger[mode] = markIfDanger(board [i] [j], j, i, 
                                   player, &numPieces, danger);
    }
}

int getDir(int * directions)
{
    for(int i = 0; i < 8; i++)
    {
        if(directions[i] == 1)
        {
            return i;
        }
    }
    return -1;
}

void resetDir(int * directions, int * numDir)
{
    *numDir = 0;
    memset(directions, 0, sizeof(int) * 8);
}

int get_endCond(int * incr)
{
    switch(*incr)
    {
        case -1: return 0;
        case 1: return 7;
        default: return -1;
    }
}

void setIncr_XandY(int * dir, int * incrY, int * incrX, int player_num)
{
    switch(*dir) //for the Y increment
    {
        case N: case NW: case NE: *incrY = 1; break;
        case S: case SE: case SW: *incrY = -1; break;
        default: *incrY = 0; break;
    }
    switch(*dir) //for the X increment
    {
        case E: case SE: case NE: *incrX = 1; break;
        case W: case NW: case SW: *incrX = -1; break;
        default: *incrX = 0; break;
    }
}