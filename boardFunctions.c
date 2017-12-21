#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "boardFunctions.h"
#include "movement.h"
#include "rand.h"

//PURPOSE:: assigns all characters to the game board
//ARGUMENTS:: char * containing the game board
//
void setBoard(char board [8] [9])
{
    char pieces [8] [9] = {
        "RNBQKBNR", "PPPPPPPP",
        "________", "________", "________", "________",
        "PPPPPPPP", "RNBQKBNR"
    };
    srand(time(NULL));
    memcpy(board, pieces, sizeof(char) * 72);
}

int Check (char board [8] [9], Player * player, Player * opposing)
{
    Piece * King = findKing(player, board);
    
    Piece aroundKing [8];
    set_aroundKing(*King, aroundKing);
    
    //if checkmate is possible, the function ends here.
    if(!Enough_pieces(board, player, opposing))
    {
        printf("gahhhh?\n");
        return stale_mate;
    }
    
    //by returning check, this will not be treated as a valid move
    //**just tested it, the kings are never able to be right beside each other
    if(kings_are_2_close(player, opposing, board))
    {
        return check;
    }
    
    if(isInDanger(*King, board, opposing, player) == 1) //if current king is in check
    {
        if(can_KingMove(aroundKing, board, player, opposing) == 1)
        {
            return check;
        }
        printf("cannot move\n\n");
        
        resetDir(player->Danger, &player->numDir);

        for(int i = 0; i < 8; i++)
        {
            findCheckDirns(i, *King, board, player); 
        }
        
        return CheckMate(board, player, opposing, *King);
    }
    else 
    {
        if(can_KingMove(aroundKing, board, player, opposing) == 0)
        {
            return Stalemate(board, player, opposing);
        }
    }
    return king_is_safe;
}

//PURPOSE:: updates characters on board, to show movement.
//ARGUMENTS:: the characters at the previous and future positions
void move_Piece(char * prev, char * future) 
{
    *future = *prev;
    *prev = '_';
}

void Castle(char side, Player * player, char board [8] [9])
{
    int dir, rook_X, oneSQ_over, twoSQ_over;
            
    switch(side)
    {
        case 'L': dir = -1; rook_X = 0; break;
        case 'R': dir = 1; rook_X = 7; break;
    }
    
    Piece * King = findKing(player, board);
    Piece * Rook = findPiece(player, rook_X, King->Y);
    
    oneSQ_over = King->X + dir;
    twoSQ_over = King->X + (2 * dir);
            
    //change King
    set_Piece(King, twoSQ_over, King->Y, 1);
    move_Piece(&board[King->Y] [King->X], &board[King->Y] [twoSQ_over]);
    
    //change Rook 
    set_Piece(Rook, oneSQ_over, Rook->Y, 1);
    move_Piece(&board[King->Y] [Rook->X], &board[King->Y] [oneSQ_over]);
}

void Move(char board[8] [9], Player * player, Player * opposing, int get_input)
{
    int Y1, Y2, X1, X2, status;
    char buffer [2];
    do
    {
        //until a possible move is entered
        if(get_input == 1)
        {
            do
            {
                input(&Y1, &Y2, &X1, &X2);
            }
            while(validateInput(&Y1, &Y2, &X1, &X2, board, player, opposing, 0));
        }
        else
        {
            do
            {
                computer_move(&Y1, &Y2, &X1, &X2, board, player);
               // fgets(buffer, 2, stdin);
            }
            while(validateInput(&Y1, &Y2, &X1, &X2, board, player, opposing, 0));
        }
        
        if(X1 == 'L' || X1 == 'R') 
        {
            Castle(X1, player, board);
            return;
        }
        
        char to_remove = '_';
        Piece * dest;
        
        //if there an opposing piece is about to be captured, remove it
        if( (dest = findPiece(opposing, X2, Y2)) != NULL)
        {
            to_remove = board [Y2] [X2];
            remove_piece(board, dest);
        }
        
        Piece * src = findPiece(player, X1, Y1); //find where it's located in player's pieces
        
        move_Piece(&board [Y1] [X1], &board [Y2] [X2]);
        set_Piece(src, X2, Y2, 1);
    
        //if player remains in check, pieces are reset, until it is valid move
        status = Check(board, player, opposing);
        
        if( status == check || status == check_mate)
        {
            move_Piece(&board [Y2] [X2], &board [Y1] [X1] );
            set_Piece(src, X1, Y1, 0);
            
            board [Y2] [X2] = to_remove;
        }
    } while(status == check || status == check_mate || 
        kings_are_2_close(player, opposing, board));
    
    //last Move is only used for its coordinates, doesn't matter if moved = 1
    set_Piece(&player->lastMove[0], X1, Y1, 0);
    set_Piece(&player->lastMove[1], X2, Y2, 0);
}