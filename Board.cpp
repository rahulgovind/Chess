#include "board.h"


const int initial_board[8][8] = { { 4,  1,  0,  0,  0,  0,  -1, -4 },
                                  { 3,  1,  0,  0,  0,  0,  -1, -3 },
                                  { 2,  1,  0,  0,  0,  0,  -1, -2 },
                                  { 5,  1,  0,  0,  0,  0,  -1, -5 },
                                  { 6,  1,  0,  0,  0,  0,  -1, -6 },
                                  { 2,  1,  0,  0,  0,  0,  -1, -2 },
                                  { 3,  1,  0,  0,  0,  0,  -1, -3 },
                                  { 4,  1,  0,  0,  0,  0,  -1, -4 } };

int abs(int x)
{
    return (x>=0) ? x : -x;
}

Board::Board()
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board_matrix[i][j] = initial_board[i][j];
    previous_moves.clear();
}

//Doesn't look at which player is playing currently
bool Board::IsValidMove(int x0, int y0, int x1, int y1)
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

    //Castling
    switch(board_matrix[x0][y0])
    {
    case 6:
        if(IsValidCastle1(x0, y0, x1, y1))
            return true;
        break;
    case -6:
        if(IsValidCastle2(x0, y0, x1, y1))
            return true;
        break;
    case 1:
        if(IsValidEnPassant1(x0, y0, x1, y1) && IsIllegal1(x0, y0, x1, y1)==false)
            return true;
        break;
    case -1:
        if(IsValidEnPassant2(x0, y0, x1, y1) && IsIllegal2(x0, y0, x1, y1)==false)
            return true;
        break;
    }
    return false;

}

bool Board::PieceHasMoved(int init_x, int init_y)
{
    for(vector<moves>::iterator it = previous_moves.begin(); it!=previous_moves.end();++it)
        if(it->x0==init_x && it->y0==init_y)
            return true;
    return false;
}

bool Board::IsIllegal1(int x0, int y0, int x1, int y1)
{
    bool result = false;
    //Make move and see if king is in check
    int prev_0 = board_matrix[x0][y0];
    int prev_1 = MakeMove(x0, y0, x1, y1);

    if(IsCheck1())
        result = true;

    //Undo move
    UndoMove(prev_0, prev_1, x0, y0, x1, y1);

    return result;
}

bool Board::IsIllegal2(int x0, int y0, int x1, int y1)
{
    bool result = false;
    //Make move and see if king is in check
    int prev_0 = board_matrix[x0][y0];
    int prev_1 = MakeMove(x0, y0, x1, y1);

    if(IsCheck2())
        result = true;

    //Undo move
    UndoMove(prev_0, prev_1, x0, y0, x1, y1);
    return result;
}
bool Board::IsCheck1()
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

bool Board::IsCheck2()
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

bool Board::IsCheckmate1()
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

bool Board::IsCheckmate2()
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

bool Board::IsStalemate1()
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

bool Board::IsStalemate2()
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
bool Board::IsValidMoveBase(int x0, int y0, int x1, int y1)
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

bool Board::IsValidPawn1(int x0, int y0, int x1, int y1)
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

bool Board::IsValidPawn2(int x0, int y0, int x1, int y1)
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

bool Board::IsValidBishop1(int x0, int y0, int x1, int y1)
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


bool Board::IsValidBishop2(int x0, int y0, int x1, int y1)
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

bool Board::IsValidKnight1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0]==3 && board_matrix[x1][y1]<=0)
    {
        if((abs(x1-x0)==2 && abs(y1-y0)==1) || (abs(x1-x0)==1 && abs(y1-y0)==2))
            return true;
    }
    return false;
}

bool Board::IsValidKnight2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0]==-3 && board_matrix[x1][y1]>=0)
    {
        if((abs(x1-x0)==2 && abs(y1-y0)==1) || (abs(x1-x0)==1 && abs(y1-y0)==2))
            return true;
    }
    return false;
}

bool Board::IsValidRook1(int x0, int y0, int x1, int y1)
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

bool Board::IsValidRook2(int x0, int y0, int x1, int y1)
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

bool Board::IsValidQueen1(int x0, int y0, int x1, int y1)
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

bool Board::IsValidQueen2(int x0, int y0, int x1, int y1)
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

bool Board::IsValidKing1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == 6 && board_matrix[x1][y1]<=0 && !(x1==x0 && y1==y0))
    {
        if(abs(x1-x0)<=1 && abs(y1-y0)<=1)
            return true;
    }
    return false;
}

bool Board::IsValidKing2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == -6 && board_matrix[x1][y1]>=0 && !(x1==x0 && y1==y0))
        if(abs(x1-x0)<=1 && abs(y1-y0)<=1)
            return true;
    return false;
}

bool Board::IsValidCastle1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == 6 && (x0==4 && y0==0) && y1==0)
    {
        if(x1==6)
        {
            //Near castling
            //Check final spots and spots in between
            if(board_matrix[7][0]==4 && board_matrix[6][0]==0 && board_matrix[5][0]==0)
            {
                //Check if king is not currently in check, and the king and respective rook haven't ever moved
                if(IsCheck1()==false && PieceHasMoved(4,0)==false && PieceHasMoved(7,0)==false)
                {

                    bool result = true;

                    board_matrix[4][0] = 0;
                    board_matrix[5][0] = 6;
                    if(IsCheck1())
                        result = false;
                    board_matrix[5][0] = 0;
                    board_matrix[4][0] = 6;


                    board_matrix[4][0] = 0;
                    board_matrix[6][0] = 6;
                    if(IsCheck1())
                        result = false;
                    board_matrix[6][0] = 0;
                    board_matrix[4][0] = 6;


                    return result;
                }
            }
        }
        else if(x1==2)
        {
            //Far castling
            if(board_matrix[0][0]==4 && board_matrix[1][0]==0 && board_matrix[2][0]==0 && board_matrix[3][0]==0)
            {
                //Check if king is not currently in check, and the king and respective rook haven't ever moved
                if(IsCheck1()==false && PieceHasMoved(4,0)==false && PieceHasMoved(0,0)==false)
                {
                    bool result = true;
                    //Check if spots in between are not in check
                    board_matrix[4][0] = 0;
                    board_matrix[3][0] = 6;
                    if(IsCheck1())
                        result = false;
                    board_matrix[3][0] = 0;
                    board_matrix[4][0] = 6;


                    board_matrix[4][0] = 0;
                    board_matrix[2][0] = 6;
                    if(IsCheck1())
                        result = false;
                    board_matrix[2][0] = 0;
                    board_matrix[4][0] = 6;

                    return result;
                }
            }
        }

    }
    return false;
}


bool Board::IsValidCastle2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == -6 && (x0==4 && y0==7) && y1==7)
    {
        if(x1==6)
        {
            //Near castling
            //Check final spots and spots in between
            if(board_matrix[7][7]==-4 && board_matrix[6][7]==0 && board_matrix[5][7]==0)
            {
                //Check if king is not currently in check, and the king and respective rook haven't ever moved
                if(IsCheck2()==false && PieceHasMoved(4,7)==false && PieceHasMoved(7,7)==false)
                {

                    bool result = true;

                    board_matrix[4][7] = 0;
                    board_matrix[5][7] = -6;
                    if(IsCheck2())
                        result = false;
                    board_matrix[5][7] = 0;
                    board_matrix[4][7] = -6;


                    board_matrix[4][7] = 0;
                    board_matrix[6][7] = -6;
                    if(IsCheck2())
                        result = false;
                    board_matrix[6][7] = 0;
                    board_matrix[4][7] = -6;


                    return result;
                }
            }
        }
        else if(x1==2)
        {
            //Far castling
            if(board_matrix[0][7]==-4 && board_matrix[1][7]==0 && board_matrix[2][7]==0 && board_matrix[3][7]==0)
            {
                //Check if king is not currently in check, and the king and respective rook haven't ever moved
                if(IsCheck2()==false && PieceHasMoved(4,7)==false && PieceHasMoved(0,7)==false)
                {
                    bool result = true;
                    //Check if spots in between are not in check
                    board_matrix[4][7] = 0;
                    board_matrix[3][7] = -6;
                    if(IsCheck2())
                        result = false;
                    board_matrix[3][7] = 0;
                    board_matrix[4][7] = -6;


                    board_matrix[4][7] = 0;
                    board_matrix[2][7] = -6;
                    if(IsCheck2())
                        result = false;
                    board_matrix[2][7] = 0;
                    board_matrix[4][7] = -6;

                    return result;
                }
            }
        }

    }
    return false;
}

bool Board::IsValidEnPassant1(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] == 1 && y0==4 && y1==5)
    {
        //Check if cross move
        if(abs(x1 - x0) == 1 && board_matrix[x1][5]==0)
        {
            if(board_matrix[x1][4] == -1)
            {
                //Check if previous move is the two step forward pawn move
                moves prev_move = previous_moves.back();

                if(prev_move.x0 == x1 && prev_move.y0 == 6 && prev_move.x1 == x1 && prev_move.y1 == 4)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::IsValidEnPassant2(int x0, int y0, int x1, int y1)
{
    if(board_matrix[x0][y0] ==-1 && y0==3 && y1==2)
    {
        //Check if cross move
        if(abs(x1 - x0) == 1 && board_matrix[x1][2]==0)
        {
            if(board_matrix[x1][3] == 1)
            {
                //Check if previous move is the two step forward pawn move
                moves prev_move = previous_moves.back();

                if(prev_move.x0 == x1 && prev_move.y0 == 1 && prev_move.x1 == x1 && prev_move.y1 == 3)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

