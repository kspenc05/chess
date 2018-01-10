#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "boardFunctions.h"
#include "movement.h"
#include "rand.h"

//PURPOSE:: assigns all characters to the game board
//ARGUMENTS:: char * containing the game board
//
void setBoard(char board[8] [9])
{
    char pieces [8] [9] = {
        "RNBQKBNR", "PPPPPPPP",
        "________", "________", "________", "________",
        "PPPPPPPP", "RNBQKBNR"
    };
    srand(time(NULL));
    memcpy(board, pieces, sizeof(char) * 72);
}

int Check (char board[8] [9], Player * current, Player * enemy)
{
    Piece * King = findKing(current,board);
    
    Piece aroundKing [8];
    set_aroundKing(*King, aroundKing);
    
    //if checkmate is possible, the function ends here.
    if(!Enough_pieces(board, current, enemy))
    {
        printf("gahhhh?\n");
        return stale_mate;
    }
    
    //by returning check, this will not be treated as a valid move
    //**just tested it, the kings are never able to be right beside each other
    if(kings_are_2_close(current, enemy,board))
    {
        return check;
    }
    
    if(isInDanger(*King, board, enemy, current) == 1) //if current king is in check
    {
        if(can_KingMove(aroundKing,board, current, enemy) == 1)
        {
            return check;
        }
        //printf("cannot move\n\n");
        
        resetDir(current->Danger, &current->numDir);

        for(int i = 0; i < 8; i++)
        {
            findCheckDirns(i, *King,board, current); 
        }
        
        return CheckMate(board, current, enemy, *King);
    }
    else 
    {
        if(can_KingMove(aroundKing,board, current, enemy) == 0)
        {
            return Stalemate(board, current, enemy);
        }
    }
    return king_is_safe;
}

//PURPOSE:: updates characters on B, to show movement.
//ARGUMENTS:: the characters at the previous and future positions
void move_Piece(char * prev, char * future) 
{
    *future = *prev;
    *prev = '_';
}

void Castle(char side, Player * current, char board[8] [9])
{
    int dir, rook_X, oneSQ_over, twoSQ_over;
            
    switch(side)
    {
        case 'L': dir = -1; rook_X = 0; break;
        case 'R': dir = 1; rook_X = 7; break;
    }
    
    Piece * King = findKing(current,board);
    Piece * Rook = findPiece(current, rook_X, King->Y);
    
    oneSQ_over = King->X + dir;
    twoSQ_over = King->X + (2 * dir);
            
    //change King
    set_Piece(King, twoSQ_over, King->Y, 1);
    move_Piece(&board[King->Y] [King->X], &board[King->Y] [twoSQ_over]);
    
    //change Rook 
    set_Piece(Rook, oneSQ_over, Rook->Y, 1);
    move_Piece(&board[King->Y] [Rook->X], &board[King->Y] [oneSQ_over]);
}

//PURPOSE:: checks it the pawn can be promoted or not, after a move 
//has been shown to be legal
//
//ARGUMENTS:: the player number, the int coordinate array, and the board
void Promotion(int player_num, int Ypos, char * pawn, int get_input)
{
    char string [10];
    int match;
    
    if(*pawn != 'P')
        return;
        
    if(!(player_num == 1 && Ypos == 7) && 
       !(player_num == 2 && Ypos == 0))
        return;
    
    //to handle a CPU promotion (a purely random one)
    if(get_input != 1)
    {
        char options [4] = {'Q', 'R', 'B', 'N'};
        *pawn = options [rand() % 4];
        return;
    }
        
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
        
    *pawn = string[0];
}

void Move(char board[8] [9], Player * current, Player * enemy, int get_input)
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
            while(validateInput(&Y1, &Y2, &X1, &X2,board, current, enemy, 0));
        }
        else
        {
            do
            {
                computer_move(&Y1, &Y2, &X1, &X2,board, current);
               // fgets(buffer, 2, stdin);
            }
            while(validateInput(&Y1, &Y2, &X1, &X2,board, current, enemy, 0));
        }
        
        if(X1 == 'L' || X1 == 'R') 
        {
            Castle(X1, current,board);
            return;
        }
        
        char to_remove = '_';
        Piece * dest;
        
        //if there an enemy piece is about to be captured, remove it
        if( (dest = findPiece(enemy, X2, Y2)) != NULL)
        {
            to_remove = board[Y2] [X2];
            remove_piece(board, dest);
        }
        
        Piece * src = findPiece(current, X1, Y1); //find where it's located in player's pieces
        
        move_Piece(&board [Y1] [X1], &board [Y2] [X2]);
        set_Piece(src, X2, Y2, 1);
    
        //if player remains in check, pieces are reset, until it is valid move
        status = Check(board, current, enemy);
        
        if( status == check || status == check_mate)
        {
            move_Piece(&board [Y2] [X2], &board [Y1] [X1] );
            set_Piece(src, X1, Y1, 0);
            
            if(dest != NULL)
            {
                set_Piece(dest, X2, Y2, 0);
            }
            board[Y2] [X2] = to_remove;
        }
    } while(status == check || status == check_mate || 
        kings_are_2_close(current, enemy,board));
    
    //to check if a promotion needs to happen to a pawn
    Promotion(current->num, Y2, &board [Y2] [X2], get_input);
    
    //last Move is only used for its coordinates, doesn't matter if moved = 1
    set_Piece(&current->lastMove[0], X1, Y1, 0);
    set_Piece(&current->lastMove[1], X2, Y2, 0);
}