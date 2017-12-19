#include <stdio.h>
#include "boardFunctions.h"
#include "draw.h"

#define _noAI_ 1
#define _2AI_ 2
#define _1AI_ 0

//PURPOSE:: plays 1 entire game of chess
void chess(int AI)
{
    char board [8][9];
    int i = 0, j;

    Player player [2];
    setBoard(board);
    initPlayers(&player[0], &player[1]);
    
    //loop runs forever
    while( (j = (i+1) % 2) > -1)
    {
        printBoard(board, &player[0], &player[1], j);
        
        switch( Check(board, &player[j], &player[i]) )
        {
            AI = j;    
            printf("%d]n\n", j);
            
            case check: 
            {
                printf("Player %d\nYou are in Check\n", player[j].num); 
                break;
            }
            
            case check_mate: 
            {
                printf("Checkmate, Player %d wins!\n", player[i].num);
                return;
            }
            
            case stale_mate: printf("Draw, since both players cannot possibly achieve checkmate!\n"); return;
        }
        Move(board, &player[j], &player[i], AI);
        i = j;
    }
}

int main()
{
    char buffer [10];
    title();
    do
    {
        printf("Would you like to play? (enter the option number)\n"
               "1. 2 player\n2. 1 player\n3. No\n");
        fgets(buffer, 10, stdin);
    
        switch(buffer[0])
        {
            case '1': chess(_noAI_); break;
            case '2': chess(_1AI_); break;
            case '3': return 0;
        }
    }
    while(buffer[0] == '1' || buffer[0] == '2');
    
    return 0;
}