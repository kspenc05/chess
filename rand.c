#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rand.h"

//PURPOSE:: picks a number between 0 - 7, which is not the number given. 
// 6/7 chance of generating a number on the iteration, if not will try again.
// will keep generating random numbers until a different one from the number variable
// is given.
//ARGUMENTS:: a number between 0 - 7.
//RETURNS:: a different number between 0 - 7 than the one given.
int diff_num_btwn_0_and_7(int number)
{
    int value = number;
    while(value == number) value = rand() % 7;
    return value;
}

//Returns a randomly generated boolean value
int Bool()
{
    return rand() % 2;
}

//PURPOSE:: flips a coin to determine, if the value should be negative or not.
//ARGUMENTS:: a value to negate or not negate
//RETURNS:: a negated form of the current number, or it does not 50-50 odds.
int pos_or_neg(int val)
{
    return (Bool()) ? val * -1 : val;
}


//PURPOSE:: generates a random bishop move
//ARGUMENTS:: the current X and Y values, and two places to store the destination
//X and Y values inside.
void random_bishop_move(int X, int Y, int * X2, int * Y2)
{
    *X2 = diff_num_btwn_0_and_7(X);
    *Y2 = Y + pos_or_neg(*X2 - X);
}

//PURPOSE:: generates a random rook move.
//ARGUMENTS:: same as for the function above.
void random_rook_move(int X, int Y, int * X2, int * Y2)
{
    *X2 = (Bool()) ? diff_num_btwn_0_and_7(X) : X;
    *Y2 = (*X2 != X) ? Y : diff_num_btwn_0_and_7(Y);
}

int convertToChar(int X)
{
    return X + 'A';
}

void computer_move(int * Y1, int * Y2, int * X1, int * X2, 
    char board [8] [9], Player * player)
{
    do get_XandY(player->all[rand() % NUM_PIECES], X1, Y1); while(*X1 == -1);
    
    //printf("X: %d  Y: %d\n", *X1, *Y1);
    
    switch(board [*Y1] [*X1])
    {
        case 'P': //for a pawn move
        {
            int dir = (player->num == 1) ? 1 : -1;
            *Y2 = *Y1 + (1 + Bool()) * dir;
            
            *X2 = (Bool()) ? *X1 + pos_or_neg(1) : *X1;
            break;
        }
        case 'N': //for a knight move
        {
            int Xs [4] = {*X1 + 2, *X1 - 2, *X1 + 1, *X1 - 1};
            int Ys [4] = {*Y1 + 1, *Y1 - 1, *Y1 + 2, *Y1 - 2};
        
            int randY = rand() % 4;
            *X2 = (randY == 0 || randY == 1) ? Xs[Bool()] : Xs[Bool()];
            *Y2 = Ys[randY];
            break;
        }
        case 'B': random_bishop_move(*X1, *Y1, X2, Y2); break;
        case 'R': random_rook_move(*X1, *Y1, X2, Y2); break;
        case 'Q': 
        {
            (Bool()) ? random_bishop_move(*X1, *Y1, X2, Y2) :
                random_rook_move(*X1, *Y1, X2, Y2);
            break;
        }
        case 'K':
        {
            *X2 = (Bool()) ? *X1 + pos_or_neg(1) : *X1;
            *Y2 = (Bool()) ? *Y1 + pos_or_neg(1) : *Y1;
        
            if(*X2 == *X1)
            {
                *Y2 = *Y1 + pos_or_neg(1);
            }
            else if(*Y2 == *Y1)
            {
                *X2 = *X1 + pos_or_neg(1);
            }
            break;
        }
    }
    //printf("who am I?\n\n");
    
    //printf("I moved my '%c' ", board [*Y1][*X1]);
    
    //printf("piece : %c SRC: X:%d Y:%d DEST: X:%d Y:%d\n",
    //    board [*Y1] [*X1], *X1, *Y1, *X2, *Y2);
    
    (*Y1)++;
    (*Y2)++;
    *X1 = convertToChar(*X1);
    *X2 = convertToChar(*X2);
    
    
    printf("from %c%d to %c%d\n", *X1, *Y1, *X2, *Y2);
    
    const clock_t start = clock();

    clock_t current;
    do{
        current = clock();
    }while((double)(current-start)/CLOCKS_PER_SEC < 2);
    
}
