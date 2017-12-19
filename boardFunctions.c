#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "boardFunctions.h"
#include "movement.h"

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

Piece * getPiece (Player * player, int X, int Y)
{
    for(int i = 0; i < NUM_PIECES; i++)
    {
        if(Y == player->all[i].Y && X == player->all[i].X)
        {
            return &player->all[i];
        }
    }
    return NULL;
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

int convertToChar(int X)
{
    return X + 'A';
}

//PURPOSE:: flips a coin to determine, if the value should be negative or not.
//ARGUMENTS:: a value to negate or not negate
//RETURNS:: a negated form of the current number, or it does not 50-50 odds.
int randomized_negation(int val)
{
    return (rand() % 2 == 0) ? val * -1 : val;
}

//PURPOSE:: picks a number between 0 - 7, which is not the number given
//ARGUMENTS:: a number between 0 - 7.
//RETURNS:: a random number between 0 -7 which is not the current number.
int diff_num_btwn_0_and_7(int number)
{
    switch(number)
    {
        case 0:
        {
            int values [7] = {1, 2, 3, 4, 5, 6, 7};
            return values[rand() % 7];
        }
        case 1:
        {
            int values [7] = {0, 2, 3, 4, 5, 6, 7};
            return values[rand() % 7];
        }
        case 2:
        {
            int values [7] = {0, 1, 3, 4, 5, 6, 7};
            return values[rand() % 7];
        }
        case 3:
        {
            int values [7] = {0, 1, 2, 4, 5, 6, 7};
            return values[rand() % 7];
        }
        case 4:
        {
            int values [7] = {0, 1, 2, 3, 5, 6, 7};
            return values[rand() % 7];
        }
        case 5:
        {
            int values [7] = {0, 1, 2, 3, 4, 6, 7};
            return values[rand() % 7];
        }
        case 6:
        {
            int values [7] = {0, 1, 2, 3, 4, 5, 7};
            return values[rand() % 7];
        }
        default:
        {
            int values [7] = {0, 1, 2, 3, 4, 5, 6};
            return values[rand() % 7];
        }
    }
}

void random_bishop_move(int X, int Y, int * X2, int * Y2)
{
    *X2 = diff_num_btwn_0_and_7(X);
    *Y2 = Y + randomized_negation(*X2 - X);
    
}

void random_rook_move(int X, int Y, int * X2, int * Y2)
{
    *X2 = (rand() % 2 == 0) ? diff_num_btwn_0_and_7(X) : X;
    *Y2 = (*X2 != X) ? Y : diff_num_btwn_0_and_7(Y);
}

void computer_move(int * Y1, int * Y2, int * X1, int * X2, 
    char board [8] [9], Player * player, Player * opposing)
{
    int pos;
    
    do{
        pos = rand() % NUM_PIECES;
    
        *Y1 = player->all[pos].Y;
        *X1 = player->all[pos].X;
    } while(*X1 == -1);
    
   // printf("char is %c\n\n", board [*Y1][*X1]);
    
    if(board [*Y1] [*X1] == 'P') //for a pawn move
    {
        int dir = (player->num == 1) ? 1 : -1;
            
        int Ys [2] = {*Y1 + dir, *Y1 + (dir * 2)};
        int Xs [3] = {*X1, *X1 - 1, *X1 + 1};
        
        int randY = rand() % 2;
        int randX = (randY == 1) ? 0 : rand() % 2 + 1;
        
        *X2 = Xs[randX];
        
        if(randX == 2)
        {
            randY = 0;
        }
        *Y2 = Ys[randY];
    }
    else if(board [*Y1] [*X1] == 'N') //for a knight move
    {
        int Xs [4] = {*X1 + 2, *X1 - 2, *X1 + 1, *X1 - 1};
        int Ys [4] = {*Y1 + 1, *Y1 - 1, *Y1 + 2, *Y1 - 2};
        
        int randY = rand() % 4;
        int randX = (randY == 0 || randY == 1) ? (rand() % 2) + 2: (rand() % 2) - 2;
        
        *Y2 = Ys[randY];
        *X2 = Xs[randX];
    }
    else if(board [*Y1] [*X1] == 'B')
    {
        random_bishop_move(*X1, *Y1, X2, Y2);
    }
    else if(board [*Y1] [*X1] == 'R')
    {
        random_rook_move(*X1, *Y1, X2, Y2);
    }
    else if(board [*Y1] [*X1] == 'Q')
    {
        (rand() % 2 == 0) ? random_bishop_move(*X1, *Y1, X2, Y2) :
            random_rook_move(*X1, *Y1, X2, Y2);
    }
    else if(board [*Y1] [*X1] == 'K')
    {
        int Xs [3] = {*X1 - 1, *X1, *X1 + 1};
        int Ys [3] = {*Y1 - 1, *Y1, *Y1 + 1};
        
        int otherX [2] = {*X1 - 1, *X1 + 1};
        int otherY [2] = {*Y1 - 1, *Y1 + 1};
        
        *X2 = Xs[rand() % 3];
        *Y2 = Ys[rand() % 3];
        
        if(*X2 == *X1)
        {
            while(*Y2 == *Y1) *Y2 = otherY[rand() % 2];
        }
        
        if(*Y2 == *Y1)
        {
            while(*X2 == *X1) *X2 = otherX[rand() % 2];
        }
    }
    
    //printf("I moved my '%c' ", board [*Y1][*X1]);
    
    //printf("piece : %c SRC: X:%d Y:%d DEST: X:%d Y:%d\n",
    //    board [*Y1] [*X1], *X1, *Y1, *X2, *Y2);
    
    (*Y1)++;
    (*Y2)++;
    *X1 = convertToChar(*X1);
    *X2 = convertToChar(*X2);
    
    /*
    printf("from %c%d to %c%d\n", *X1, *Y1, *X2, *Y2);
    
    const clock_t start = clock();

    clock_t current;
    do{
        current = clock();

    }while((double)(current-start)/CLOCKS_PER_SEC < 2);
    */
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
                computer_move(&Y1, &Y2, &X1, &X2, board, player, opposing);
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