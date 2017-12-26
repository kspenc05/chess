#include "player.h"
#include <string.h>

void set_Piece (Piece * piece, int X, int Y, int moved)
{
    Piece copy = {X, Y, moved};
    *piece = copy;
}

void get_XandY (Piece piece, int * X, int * Y)
{
    *X = piece.X;
    *Y = piece.Y;
}

Piece * findPiece(Player * player, int X, int Y)
{
    for(int i = 0; i < NUM_PIECES; i++)
    {
        //if the opposing player is about to lose a piece
        if(Y == player->all[i].Y && X == player->all[i].X) 
        {
            return &player->all[i];
        }
    }
    return NULL;
}

int match(char board [8] [9], int X, int Y, char type)
{
    return (board [Y] [X] == type) ? 1 : 0;
}

Piece * findKing(Player * player, char board [8] [9])
{
    for(int i = 0; i < NUM_PIECES; i++)
    {
        if(player->all[i].Y != -1 &&
            board [ player->all[i].Y][ player->all[i].X] == 'K')
        {
            return &player->all[i];
        }
    }
    return NULL;
}

void remove_piece(char board [8] [9], Piece * piece)
{
    board [ piece->Y ] [ piece->X ] = '_';
    set_Piece(piece, -1, -1, 0);
}

//PURPOSE:: assigns the Pieces of 1 set of pieces per player.
//ARGUMENTS:: the player, and the Y position where the first row is located
//Note:: adds two entire rows of Pieces from Ypos to Ypos + 1.
void init_player(Player * player, int Ypos, int playerNum)
{
    player->num = playerNum;
    
    //set every piece's coordinates 
    for(int i = 0; i < NUM_PIECES; i++)
    {
        set_Piece(&player->all[i], i % 8, Ypos, 0);
        
        if(i == 7)
            Ypos++;
    }
    player->numDir = 0;
    memset(player->Danger, 0, sizeof(int) * 8);
    
    set_Piece(&player->lastMove[0], -1, -1, 0);
    set_Piece(&player->lastMove[1], -1, -1, 0);
}

void initPlayers(Player * player1, Player * player2)
{
    init_player(player1, 0, 1);
    init_player(player2, 6, 2);
}