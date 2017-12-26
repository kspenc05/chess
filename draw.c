#include <stdio.h>
#include "draw.h"

#define Background_black "\x1B[40m"

#define Gray "\x1B[30;1m"
#define Red "\x1B[31;1m"
#define Green "\x1B[32;1m"
#define Yellow "\x1B[33;1m"
#define Blue "\x1B[34;1m"
#define Purple "\x1B[35;1m"
#define Cyan "\x1B[36;1m"
#define White "\x1B[37;1m"
#define Reset "\x1B[0m"

void title()
{
    printf(Red    " ________  \t          \t _________\t  _______\t   ________\n"
           Yellow "/          \t|       | \t|        \t  |       \t  |\n" 
           Green  "|          \t|       | \t|        \t  |       \t  |\n"
           Cyan   "|          \t|_______| \t|        \t  |_______\t  |_______\n"
           Blue   "|          \t|       | \t|________\t          |\t         |\n"
           Gray   "|          \t|       | \t|        \t          |\t         |\n"
           White "|          \t|       | \t|        \t          |\t         |\n"
           Purple   "\\________  \t|       | \t|________\t   _______|\t  _______|\n" Yellow);
}

//PURPOSE:: determines loop conditions, based on current direction
//ARGUMENTS:: variable for endCondition, start and the int direction.
void setLoopConditions(int * endCondition, int * start, int direction)
{
    *endCondition = (direction == FORWARD) ? 8 : -1;
    *start = (direction == FORWARD) ? -1 : 8;
}

//PURPOSE:: prints the same character 'n' times,
//ARGUMENTS:: number of times to print, and the character to print
void printThisChar(char print, int times)
{
    for(int i = 0; i < times; i++)
        printf(White "%c", print);
}

//PURPOSE:: prints letter co-ordinates for the user to know regarding position
//ARGUMENTS:: current loop condition, loop counter, and direction.
void printLetters(int direction)
{
    printThisChar(' ', 7);
    for(int i = 0; i < 8; i++)
    {
        printf(Yellow "%c   ", "ABCDEFGH"[i]);
    }
    printf("    \n");
}

//PURPOSE:: prints out all board locations and number co-ordinates for user
//ARGUMENTS:: the char array for the board, and the direction to print it
void printPieces(char board [8] [9], Player * P1, Player * P2,
    int direction)
{
    int condition, i, incr;
    setLoopConditions(&condition, &i, direction);
    
    incr = (direction == FORWARD) ? 1 : -1;
    
    while( (i = i + incr) != condition)
    {
        for(int j = 0; j < 8; j++)
        {
            if(j == 0)
            {
                printf(Yellow "%d", i + 1);
                printf(White " ||| " Yellow);
            }
            if(findPiece(P1, j, i) != NULL)
            {
                printf(Cyan " %c  " Yellow, board[i][j]);
            }
            else if(findPiece(P2, j, i) != NULL)
            {
                printf(Green " %c  " Yellow, board[i][j]);
            }
            else
                printf(Yellow " %c  ", board[i][j]);
            
            if(j == 7)
            {
                printf(White "||| ");
                printf(Yellow "%d\n", i + 1);
            }
        }
        
        //NOTE:: added so it would stop printing 1 wall too long on both sides
        if(i != condition - 1 && i != condition + 1)
        {
            printf(White "  |||");
            printThisChar(' ', B_width - 6); 
            printf(White "|||  \n" Yellow);
        }
    }  
}

//PURPOSE:: prints horizontal border line for top or bottom of board
//ARGUMENTS:: none
void printHorizontalBorder()
{
    printf("  ");
    printThisChar('%', B_width);
    printf("  \n");
}

void printBoard(char board [8] [9], Player * P1, Player * P2, int direction)
{
    printf(Background_black);
    printLetters(direction);
    printHorizontalBorder();
    
    printPieces(board, P1, P2, direction);
    
    printHorizontalBorder();
    printLetters(direction);
    printf("\n");
}