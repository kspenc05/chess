#include "check.h"

int CheckMate (char board [8] [9], Player * player, Player * opposing, Piece King)
{
    if(player->numDir == 1) //if there is more than one attackable direction, cannot be blocked
    {
        int incr1, incr2, dir = getDir(player->Danger);
        int i = King.Y, j = King.X; 
        char other_danger;
        
        setIncr_XandY(&dir, &incr1, &incr2, player->num);
        other_danger = setDanger(&dir);
        
        while(board [i] [j] != 'Q' && 
            board [i] [j] != other_danger)
        {
            i = i + incr1;
            j = j + incr2;
            
            Piece targetSQ = {j, i, 0};
            if(isInDanger(targetSQ, board, player, opposing) == 1)
            {
                return check;
            }
        } 
    }
    else if(player->numDir == 0) //if no directions being attacked from
    {
        for(int i = 0; i < NUM_PIECES; i++)
        {
            if(opposing->all[i].X != -1 &&
                board [opposing->all[i].Y] [opposing->all[i].X] == 'N' &&
                isInDanger(opposing->all[i], board, player, opposing) == 1)
            {
                int coords[4] = {opposing->all[i].X, opposing->all[i].Y,
                    King.X, King.Y};
                    
                if(valid_Knight_move(coords) == move_true)
                {
                    return check;
                }
            }
        }
    }
    return check_mate; //if it is not possible to block check
}

void set_aroundKing(Piece King, Piece aroundKing [num_direcns])
{
    int k = 0;
    
    //set vertical sq's
    set_Piece(&aroundKing[k++], King.X, King.Y - 1, 0);
    set_Piece(&aroundKing[k++], King.X, King.Y + 1, 0);
    
    //set horizontal sq's
    set_Piece(&aroundKing[k++], King.X - 1, King.Y, 0);
    set_Piece(&aroundKing[k++], King.X + 1, King.Y, 0);
    
    //set diagonal sq's
    set_Piece(&aroundKing[k++], King.X + 1, King.Y + 1, 0);
    set_Piece(&aroundKing[k++], King.X - 1, King.Y - 1, 0);
    
    set_Piece(&aroundKing[k++], King.X + 1, King.Y - 1, 0);
    set_Piece(&aroundKing[k++], King.X - 1, King.Y + 1, 0);
}

int kings_are_2_close (Player * player, Player * opposing, char board [8] [9])
{
    Piece * myKing = findKing(player, board);
    Piece * theirKing = findKing(opposing, board);
    
    int diffX = myKing->X - theirKing->X;
    int diffY = myKing->Y - theirKing->Y; 
    
    return ( (diffY == 0 || diffY == -1 || diffY == 1) && 
             (diffX == 0 || diffX == -1 || diffX == 1) ) ? 1 : 0;
}

int can_KingMove (Piece aroundKing[num_direcns], char board [8] [9],
    Player * player, Player * opposing)
{
    for(int i = 0; i < num_direcns; i++)
    {
        if(withinBounds(aroundKing[i].X) == 0 &&
            withinBounds(aroundKing[i].Y) == 0 &&
            friendlyFire(aroundKing[i].X, aroundKing[i].Y, player, 0) == 0 &&
            isInDanger(aroundKing[i], board, opposing, player) == 0) 
        {
            return 1;
        }
    }
    return 0; //returns 0, if king cannot move
}

//PURPOSE:: counts the number of pieces for the player
//ARGUMENTS:: board and player variables
//RETURNS:: 1 if sufficient pieces for checkmate, otherwise 0.
int count_pieces (char board [8] [9], Player * player)
{
    int pawns = 0;
    int queens = 0;
    int bishops = 0;
    int rooks = 0;
    int knights = 0;
    
    for(int i = 0; i < NUM_PIECES; i++)
    {
        Piece target = player->all[i];
        
        if(player->all[i].Y != -1)
        {
            switch (board [target.Y] [target.X])
            {
                case 'P': pawns++; break;
                case 'R': rooks++; break;
                case 'N': knights++; break;
                case 'B': bishops++; break;
                case 'Q': queens++; break;
            }
        }
    }
    
    return (pawns > 0 || queens > 0 || rooks > 0 ||
        (knights >= 1 && bishops >= 1) || bishops >= 2 ||
        knights >= 3) ? 1 : 0;
}

//PURPOSE:: determines if checkmate is possible, by ensuring there are
//enough pieces for checkmate.
//ARGUMENTS:: the board and both player variables.
//RETURNS:: 1 if there is sufficient pieces for either player to achieve
//checkmate, otherwise 0.
int Enough_pieces(char board [8] [9], Player * player, Player * opposing)
{
    return (count_pieces(board, player) || 
        count_pieces(board, opposing) ) ? 1 : 0;
}

//PURPOSE:: sets the pieces to check possible moves for stalemate
//
//ARGUMENTS:: the type of piece, an array of pieces to store moves in,
// and the target piece
//
//RETURNS:: possible moves to check for any given piece
//

int set_move_able (char piece, Piece * move_able, Piece * target)
{
    int k = 0;
    
    switch(piece)
    {
        case 'P':
        {
            //set all pawn moves, northwest 1 sq, northeast 1 sq, forward 1 sq.
            
            set_Piece(&move_able[k++], target->X - 1, target->Y + 1, 0);
            set_Piece(&move_able[k++], target->X + 1, target->Y + 1, 0);
            set_Piece(&move_able[k++], target->X, target->Y + 1, 0);
    
            if(target->moved == 0)
            {
                set_Piece(&move_able[k++], target->X, target->Y + 2, 0);
            }
            break;
        }
        case 'R':
        {
            //set all non-diagonals 1 square from current pos
            set_Piece(&move_able[k++], target->X - 1, target->Y, 0);
            set_Piece(&move_able[k++], target->X + 1, target->Y, 0);

            set_Piece(&move_able[k++], target->X, target->Y + 1, 0);
            set_Piece(&move_able[k++], target->X, target->Y - 1, 0);
            k = 4;
            
            break;
        }
        case 'B':
        {
            //set all diagonals 1 square from current pos
            set_Piece(&move_able[k++], target->X + 1, target->Y + 1, 0);
            set_Piece(&move_able[k++], target->X - 1, target->Y - 1, 0);
            
            set_Piece(&move_able[k++], target->X + 1, target->Y - 1, 0);
            set_Piece(&move_able[k++], target->X - 1, target->Y + 1, 0);
            k = 4;
            break;
        }
        case 'Q':
        {
            set_aroundKing(*target, move_able);
            k = 8;
            break;
        }
        case 'N':
        {
            //set all vertical by 2 knight moves
            set_Piece(&move_able[k++], target->X + 1, target->Y - 2, 0);
            set_Piece(&move_able[k++], target->X + 1, target->Y + 2, 0);
            
            set_Piece(&move_able[k++], target->X - 1, target->Y - 2, 0);
            set_Piece(&move_able[k++], target->X - 1, target->Y + 2, 0);
            
            //set all horizontal by 2 knight moves
            set_Piece(&move_able[k++], target->X + 2, target->Y - 1, 0);
            set_Piece(&move_able[k++], target->X + 2, target->Y + 1, 0);
            
            set_Piece(&move_able[k++], target->X - 2, target->Y - 1, 0);
            set_Piece(&move_able[k++], target->X - 2, target->Y + 1, 0);
            break;
        }
    }
    return k;
}

int Stalemate(char board [8] [9], Player * player, Player * opposing)
{
    int num_moves, coords[4];
    Piece move_able [8], target;
    
    for(int i = 0; i < NUM_PIECES; i++)
    {
        target = player->all[i];
        get_XandY(target, &coords[0], &coords[1]);
        
        num_moves = set_move_able(board [target.Y] [target.X], 
            move_able, &target);
        
        for(int j = 0; j < num_moves; j++)
        {
            get_XandY(move_able[j], &coords[2], &coords[3]);
            
            //to check if any possible square can attack the king
            if(checkArrayBounds(coords, 0) == move_true &&
                friendlyFire(coords[2], coords[3], player, 0) == move_true &&
                isValidMove(board, coords, player, opposing, 0) == move_true)
            {
                return king_is_safe;
            }
        }
    }
    return stale_mate;
}