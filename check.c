#include "check.h"

int CheckMate (char B [8] [9], Player * P1, Player * P2, Piece King)
{
    if(P1->numDir == 1) //if there is more than one attackable direction, cannot be blocked
    {
        int incr1, incr2, dir = getDir(P1->Danger);
        int i = King.Y, j = King.X; 
        char other_danger;
        
        setIncr_XandY(&dir, &incr1, &incr2, P1->num);
        other_danger = setDanger(&dir);
        
        while(B [i] [j] != 'Q' && 
            B [i] [j] != other_danger)
        {
            i = i + incr1;
            j = j + incr2;
            
            Piece targetSQ = {j, i, 0};
            if(isInDanger(targetSQ, B, P1, P2) == 1)
            {
                return check;
            }
        } 
    }
    else if(P1->numDir == 0) //if no directions being attacked from
    {
        for(int i = 0; i < NUM_PIECES; i++)
        {
            if(P2->all[i].X != -1 &&
                B [P2->all[i].Y] [P2->all[i].X] == 'N' &&
                isInDanger(P2->all[i], B, P1, P2) == 1)
            {
                int coords[4] = {P2->all[i].X, P2->all[i].Y,
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

int kings_are_2_close (Player * P1, Player * P2, char B [8] [9])
{
    Piece * myKing = findKing(P1, B);
    Piece * theirKing = findKing(P2, B);
    
    int diffX = myKing->X - theirKing->X;
    int diffY = myKing->Y - theirKing->Y; 
    
    return ( (diffY == 0 || diffY == -1 || diffY == 1) && 
             (diffX == 0 || diffX == -1 || diffX == 1) ) ? 1 : 0;
}

int can_KingMove (Piece aroundKing[num_direcns], char B [8] [9],
    Player * P1, Player * P2)
{
    for(int i = 0; i < num_direcns; i++)
    {
        if(withinBounds(aroundKing[i].X) == 0 &&
            withinBounds(aroundKing[i].Y) == 0 &&
            friendlyFire(aroundKing[i].X, aroundKing[i].Y, P1, 0) == 0 &&
            isInDanger(aroundKing[i], B, P2, P1) == 0) 
        {
            return 1;
        }
    }
    return 0; //returns 0, if king cannot move
}

//PURPOSE:: counts the number of pieces for the player
//ARGUMENTS:: board and player variables
//RETURNS:: 1 if sufficient pieces for checkmate, otherwise 0.
int count_pieces (char B [8] [9], Player * P1)
{
    int pawns = 0;
    int queens = 0;
    int bishops = 0;
    int rooks = 0;
    int knights = 0;
    
    for(int i = 0; i < NUM_PIECES; i++)
    {
        Piece target = P1->all[i];
        
        if(P1->all[i].Y != -1)
        {
            switch (B [target.Y] [target.X])
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
int Enough_pieces(char B [8] [9], Player * P1, Player * P2)
{
    return (count_pieces(B, P1) || count_pieces(B, P2) ) ? 1 : 0;
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
            break;
        }
        case 'B':
        {
            //set all diagonals 1 square from current pos
            set_Piece(&move_able[k++], target->X + 1, target->Y + 1, 0);
            set_Piece(&move_able[k++], target->X - 1, target->Y - 1, 0);
            
            set_Piece(&move_able[k++], target->X + 1, target->Y - 1, 0);
            set_Piece(&move_able[k++], target->X - 1, target->Y + 1, 0);
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

int Stalemate(char B [8] [9], Player * P1, Player * P2)
{
    int num_moves, coords[4];
    Piece move_able [8], target;
    
    for(int i = 0; i < NUM_PIECES; i++)
    {
        target = P1->all[i];
        get_XandY(target, &coords[0], &coords[1]);
        
        num_moves = set_move_able(B [target.Y] [target.X], 
            move_able, &target);
        
        for(int j = 0; j < num_moves; j++)
        {
            get_XandY(move_able[j], &coords[2], &coords[3]);
            
            //to check if any possible square can attack the king
            if(checkArrayBounds(coords, 0) == move_true &&
                friendlyFire(coords[2], coords[3], P1, 0) == move_true &&
                isValidMove(B, coords, P1, P2, 0) == move_true)
            {
                return king_is_safe;
            }
        }
    }
    return stale_mate;
}