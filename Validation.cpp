#include "engine.h"

int abs(int x)
{
    return (x>=0) ? x : -x;
}

//Doesn't look at current player
bool Engine::IsValidMove(int x0, int y0, int x1, int y1)
{
    if(IsValidMoveBase(x0, y0, x1, y1))
    {
        if(board_matrix[x0][y0]>0)
        {
            //Remove illegal move for player 1
            if(IsIllegal1(x0, y0, x1, y1))
                return false;
            else
                return true;
        }
        else if(board_matrix[x0][y0]<0)
        {
            //Remove illegal move for player 2
            if(IsIllegal2(x0, y0, x1, y1))
                return false;
            else
                return true;
        }
    }
    return false;

}

bool Engine::IsIllegal1(int x0, int y0, int x1, int y1)
{
    bool result = false;
    //Make move and see if king is in check
    int prev_0 = board_matrix[x0][y0];
    int prev_1 = board_matrix[x1][y1];

    //Make move
    board_matrix[x1][y1] = prev_0;
    board_matrix[x0][y0] = 0;

    if(IsCheck1())
        result = true;

    //Undo move
    board_matrix[x1][y1] = prev_1;
    board_matrix[x0][y0] = prev_0;

    return result;
}

bool Engine::IsIllegal2(int x0, int y0, int x1, int y1)
{
    bool result = false;
    //Make move and see if king is in check
    int prev_0 = board_matrix[x0][y0];
    int prev_1 = board_matrix[x1][y1];

    //Make move
    board_matrix[x1][y1] = prev_0;
    board_matrix[x0][y0] = 0;

    if(IsCheck2())
        result = true;

    //Undo move
    board_matrix[x1][y1] = prev_1;
    board_matrix[x0][y0] = prev_0;

    return result;
}
bool Engine::IsCheck1()
{
    //Find position of king
    int king_pos_x, king_pos_y;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(board_matrix[i][j]==6)
            {
                king_pos_x = i;
                king_pos_y = j;
            }

    //Iterate over all possible positions of opponent
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(board_matrix[i][j]<0)
            {
                if(IsValidMoveBase(i,j,king_pos_x, king_pos_y))
                    return true;
            }
    return false;
}

bool Engine::IsCheck2()
{
    //Find position of king
    int king_pos_x, king_pos_y;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(board_matrix[i][j]==-6)
            {
                king_pos_x = i;
                king_pos_y = j;
            }

    //Iterate over all possible positions of opponent
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(board_matrix[i][j]>0)
            {
                if(IsValidMoveBase(i,j,king_pos_x, king_pos_y))
                    return true;
            }
    return false;
}

bool Engine::IsCheckmate1()
{
    //Check if currently in check
    if(IsCheck1())
    {
        //Check if any move is valid
        //Since no move can be valid while the king is in check
        for(int x0=0;x0<8;x0++)
            for(int y0=0;y0<8;y0++)
                for(int x1=0;x1<8;x1++)
                    for(int y1=0;y1<8;y1++)
                    {
                        if(board_matrix[x0][y0]>0)
                            if(IsValidMove(x0,y0, x1, y1))
                                return false;
                    }
        return true;
    }
    return false;
}

bool Engine::IsCheckmate2()
{
    //Check if currently in check
    if(IsCheck2())
    {
        //Check if any move is valid
        //Since no move can be valid while the king is in check
        for(int x0=0;x0<8;x0++)
            for(int y0=0;y0<8;y0++)
                for(int x1=0;x1<8;x1++)
                    for(int y1=0;y1<8;y1++)
                    {
                        if(board_matrix[x0][y0]<0)
                            if(IsValidMove(x0,y0, x1, y1))
                                return false;
                    }
        return true;
    }
    return false;
}

bool Engine::IsStalemate1()
{
    //Check if currently in check
    if(!IsCheck1())
    {
        //Check if any move is valid
        //Since no move can be valid while the king is in check
        for(int x0=0;x0<8;x0++)
            for(int y0=0;y0<8;y0++)
                for(int x1=0;x1<8;x1++)
                    for(int y1=0;y1<8;y1++)
                    {
                        if(board_matrix[x0][y0]>0)
                            if(IsValidMove(x0,y0, x1, y1))
                                return false;
                    }
        return true;
    }
    return false;
}

bool Engine::IsStalemate2()
{
    //Check if currently in check
    if(!IsCheck2())
    {
        //Check if any move is valid
        //Since no move can be valid while the king is in check
        for(int x0=0;x0<8;x0++)
            for(int y0=0;y0<8;y0++)
                for(int x1=0;x1<8;x1++)
                    for(int y1=0;y1<8;y1++)
                    {
                        if(board_matrix[x0][y0]<0)
                            if(IsValidMove(x0,y0, x1, y1))
                                return false;
                    }
        return true;
    }
    return false;
}
bool Engine::IsValidMoveBase(int x0, int y0, int x1, int y1)
{
    int character_index = board_matrix[x0][y0];
    switch(character_index)
    {
        case 1: return IsValidPawn1(x0, y0, x1, y1);
        case 2: return IsValidBishop1(x0, y0, x1, y1);
        case 3: return IsValidKnight1(x0, y0, x1, y1);
        case 4: return IsValidRook1(x0, y0, x1, y1);
        case 5: return IsValidQueen1(x0, y0, x1, y1);
        case 6: return IsValidKing1(x0, y0, x1, y1);

        case -1: return IsValidPawn2(x0, y0, x1, y1);
        case -2: return IsValidBishop2(x0, y0, x1, y1);
        case -3: return IsValidKnight2(x0, y0, x1, y1);
        case -4: return IsValidRook2(x0, y0, x1, y1);
        case -5: return IsValidQueen2(x0, y0, x1, y1);
        case -6: return IsValidKing2(x0, y0, x1, y1);

    }

    return false;
}

bool Engine::IsValidPawn1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == 1)
    {
        if(x1 == x0)
        {
            //Check moves ahead
            if(y1==y0+1)
            {
                if(board_matrix[x1][y1]==0)
                {
                    //Single move ahead
                    return true;
                }
            }
            else if(y1==y0+2 && y0 == 1)
            {
                if(board_matrix[x1][y1]==0 && board_matrix[x0][y0+1]==0)
                {
                    //Two moves ahead
                    return true;
                }
            }
        }
        else if(abs(x1-x0)==1 && (y1-y0)==1)
        {
            if(board_matrix[x1][y1]<0)
            {
                //Cross move capture
                return true;
            }
        }
    }
    return false;
}

bool Engine::IsValidPawn2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == -1)
    {
        if(x1 == x0)
        {
            //Check moves ahead
            if(y1==y0-1)
            {
                if(board_matrix[x1][y1]==0)
                {
                    //Single move ahead
                    return true;
                }
            }
            else if(y1==y0-2 && y0 == 6)
            {
                if(board_matrix[x1][y1]==0 && board_matrix[x0][y0-1]==0)
                {
                    //Two moves ahead
                    return true;
                }
            }
        }
        else if(abs(x1-x0)==1 && (y0-y1)==1)
        {
            if(board_matrix[x1][y1]>0)
            {
                //Cross move capture
                return true;
            }
        }
    }
    return false;
}

bool Engine::IsValidBishop1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == 2 && board_matrix[x1][y1]<=0)
    {
        if(abs(y1-y0) == abs(x1-x0) && x1-x0 != 0)
        {
            int x_slope = (x1>x0) ? 1 : -1;
            int y_slope = (y1>y0) ? 1 : -1;

            for(int x = x0 + x_slope, y = y0+y_slope; ;x+=x_slope, y+=y_slope)
            {
                if(x==x1 && y==y1)
                    return true;
                else if(board_matrix[x][y]!=0)
                    return false;

            }
        }
    }

    return false;
}


bool Engine::IsValidBishop2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == -2 && board_matrix[x1][y1]>=0)
    {
        if(abs(y1-y0) == abs(x1-x0) && x1-x0 != 0)
        {
            int x_slope = (x1>x0) ? 1 : -1;
            int y_slope = (y1>y0) ? 1 : -1;

            for(int x = x0 + x_slope, y = y0+y_slope; ;x+=x_slope, y+=y_slope)
            {
                if(x==x1 && y==y1)
                    return true;
                else if(board_matrix[x][y]!=0)
                    return false;

            }
        }
    }

    return false;
}

bool Engine::IsValidKnight1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0]==3 && board_matrix[x1][y1]<=0)
    {
        if((abs(x1-x0)==2 && abs(y1-y0)==1) || (abs(x1-x0)==1 && abs(y1-y0)==2))
            return true;
    }
    return false;
}

bool Engine::IsValidKnight2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0]==-3 && board_matrix[x1][y1]>=0)
    {
        if((abs(x1-x0)==2 && abs(y1-y0)==1) || (abs(x1-x0)==1 && abs(y1-y0)==2))
            return true;
    }
    return false;
}

bool Engine::IsValidRook1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0]==4 && board_matrix[x1][y1]<=0)
    {
        if(x0 == x1 && y0!=y1)
        {
            //Check in the upward direction
            int y_slope = ( y1 > y0 ) ? 1 : -1;
            for(int y = y0 + y_slope; ; y+=y_slope)
            {
                if(y==y1)
                    return true;
                else if(board_matrix[x0][y]!=0)
                    return false;
            }
        }
        else if(x0 != x1 && y0==y1)
        {
            //Check in the downward direction
            int x_slope = ( x1 > x0 ) ? 1 : -1;
            for(int x = x0 + x_slope; ; x+=x_slope)
            {
                if(x==x1)
                    return true;
                else if(board_matrix[x][y0]!=0)
                    return false;
            }
        }
    }
    return false;
}

bool Engine::IsValidRook2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0]==-4 && board_matrix[x1][y1]>=0)
    {
        if(x0 == x1 && y0!=y1)
        {
            //Check in the upward direction
            int y_slope = ( y1 > y0 ) ? 1 : -1;
            for(int y = y0 + y_slope; ; y+=y_slope)
            {
                if(y==y1)
                    return true;
                else if(board_matrix[x0][y]!=0)
                    return false;
            }
        }
        else if(x0 != x1 && y0==y1)
        {
            //Check in the downward direction
            int x_slope = ( x1 > x0 ) ? 1 : -1;
            for(int x = x0 + x_slope; ; x+=x_slope)
            {
                if(x==x1)
                    return true;
                else if(board_matrix[x][y0]!=0)
                    return false;
            }
        }
    }
    return false;
}

bool Engine::IsValidQueen1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == 5 && board_matrix[x1][y1]<=0)
    {
        //Combination of bishop and rook code
        if(x0 == x1 && y0!=y1)
        {
            //Check in the upward direction
            int y_slope = ( y1 > y0 ) ? 1 : -1;
            for(int y = y0 + y_slope; ; y+=y_slope)
            {
                if(y==y1)
                    return true;
                else if(board_matrix[x0][y]!=0)
                    return false;
            }
        }
        else if(x0 != x1 && y0==y1)
        {
            //Check in the downward direction
            int x_slope = ( x1 > x0 ) ? 1 : -1;
            for(int x = x0 + x_slope; ; x+=x_slope)
            {
                if(x==x1)
                    return true;
                else if(board_matrix[x][y0]!=0)
                    return false;
            }
        }
        else if(abs(y1-y0) == abs(x1-x0) && x1-x0 != 0)
        {
            int x_slope = (x1>x0) ? 1 : -1;
            int y_slope = (y1>y0) ? 1 : -1;

            for(int x = x0 + x_slope, y = y0+y_slope; ;x+=x_slope, y+=y_slope)
            {
                if(x==x1 && y==y1)
                    return true;
                else if(board_matrix[x][y]!=0)
                    return false;

            }
        }
    }
    return false;
}

bool Engine::IsValidQueen2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == -5 && board_matrix[x1][y1]>=0)
    {
        //Combination of bishop and rook code
        if(x0 == x1 && y0!=y1)
        {
            //Check in the upward direction
            int y_slope = ( y1 > y0 ) ? 1 : -1;
            for(int y = y0 + y_slope; ; y+=y_slope)
            {
                if(y==y1)
                    return true;
                else if(board_matrix[x0][y]!=0)
                    return false;
            }
        }
        else if(x0 != x1 && y0==y1)
        {
            //Check in the downward direction
            int x_slope = ( x1 > x0 ) ? 1 : -1;
            for(int x = x0 + x_slope; ; x+=x_slope)
            {
                if(x==x1)
                    return true;
                else if(board_matrix[x][y0]!=0)
                    return false;
            }
        }
        else if(abs(y1-y0) == abs(x1-x0) && x1-x0 != 0)
        {
            int x_slope = (x1>x0) ? 1 : -1;
            int y_slope = (y1>y0) ? 1 : -1;

            for(int x = x0 + x_slope, y = y0+y_slope; ;x+=x_slope, y+=y_slope)
            {
                if(x==x1 && y==y1)
                    return true;
                else if(board_matrix[x][y]!=0)
                    return false;

            }
        }
    }
    return false;
}

bool Engine::IsValidKing1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == 6 && board_matrix[x1][y1]<=0)
        if(abs(x1-x0)<=1 && abs(y1-y0)<=1)
            return true;
    return false;
}

bool Engine::IsValidKing2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == -6 && board_matrix[x1][y1]>=0)
        if(abs(x1-x0)<=1 && abs(y1-y0)<=1)
            return true;
    return false;
}
