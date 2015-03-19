#include "engine.h"

#include <fstream>

int initial_board[8][8] ={{ 4,  1,  0,  0,  0,  0,  -1, -4 },
                          { 3,  1,  0,  0,  0,  0,  -1, -3 },
                          { 2,  1,  0,  0,  0,  0,  -1, -2 },
                          { 5,  1,  0,  0,  0,  0,  -1, -5 },
                          { 6,  1,  0,  0,  0,  0,  -1, -6 },
                          { 2,  1,  0,  0,  0,  0,  -1, -2 },
                          { 3,  1,  0,  0,  0,  0,  -1, -3 },
                          { 4,  1,  0,  0,  0,  0,  -1, -4 } };

Engine::Engine(bool AI_mode)
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board_matrix[i][j] = initial_board[i][j];
    player1 = true;
    game_status = GAME_NORMAL;

    prev_ai_move.x0 = -1;
    prev_ai_move.y0 = -1;
    prev_ai_move.x1 = -1;
    prev_ai_move.y1 = -1;

    ai_mode = AI_mode;
    previous_moves.clear();


}

int Engine::GetGameStatus()
{
    return game_status;
}

int Engine::GetCurrentPlayer()
{
    if(player1)
        return 1;
    else return 2;
}
int Engine::GetPiece(int x, int y)
{
    return board_matrix[x][y];
}

void Engine::ProcessInput(int x0, int y0, int x1, int y1)
{
    if((player1 && board_matrix[x0][y0]>0) || (!player1 && board_matrix[x0][y0]<0))
        if(IsValidMove(x0, y0, x1, y1))
        {
            if(ENGINE_DEBUG)
            {
                try
                {
                    ofstream dbg;
                    if(previous_moves.size()==0)
                        dbg.open("debug.dat", ios::trunc);
                    else
                        dbg.open("debug.dat", ios::app);
                    dbg<<x0<<"\t"<<y0<<"\t"<<x1<<"\t"<<y1<<"\n";
                    dbg.close();
                }
                catch(...)
                {
                    cout<<"Error writing to file"<<endl;
                }
            }
            MakeMove(x0, y0, x1, y1);
            player1 = !player1;

            if((player1 && IsCheck1()) || (!player1 && IsCheck2()))
                game_status = GAME_CHECK;
            else
                game_status = GAME_NORMAL;

            if((player1 && IsCheckmate1()) || (!player1 && IsCheckmate2()))
                game_status = GAME_CHECKMATE;
            else if((player1 && IsStalemate1()) || (!player1 && IsStalemate2()))
                game_status = GAME_STALEMATE;

            //Just for debugging
            if(player1==false && ai_mode && game_status!=GAME_CHECKMATE && game_status!=GAME_STALEMATE)
            {
                game_status = GAME_THINKING;
                MakeAIMove();
            }
        }
}


int Engine::MakeMove(int x0, int y0, int x1, int y1)
{
    previous_moves.push_back(moves(x0,y0,x1,y1)); //Temporary
    int temp = board_matrix[x1][y1];
    board_matrix[x1][y1] = board_matrix[x0][y0];
    board_matrix[x0][y0] = 0;

    if(y1 == 7 && board_matrix[x1][y1] == 1)
    {
        //Handle pawn promotion
        board_matrix[x1][y1] = 5;
        temp = 7;
    }
    else if(y1 == 0 && board_matrix[x1][y1] == -1)
    {
        //Handle pawn promotion
        board_matrix[x1][y1] = -5;
        temp = -7;
    }
    else if(board_matrix[x1][y1]==6 && y1==0 && y0==0 && abs(x1-x0)==2)
    {
        //Handle Castling for player 1
        if(x1==2)
        {
            //Far castle
            board_matrix[0][0] = 0;
            board_matrix[3][0] = 4;
            temp = 8;
        }
        else if(x1==6)
        {
            //Near castle
            board_matrix[7][0] = 0;
            board_matrix[5][0] = 4;
            temp = 8;
        }
    }
    else if(board_matrix[x1][y1]==-6 && y1==7 && y0==7 && abs(x1-x0)==2)
    {
        //Handle Castling for player 2
        if(x1==2)
        {
            //Far castle
            board_matrix[0][7] = 0;
            board_matrix[3][7] = -4;
            temp = -8;
        }
        else if(x1==6)
        {
            //Near castle
            board_matrix[7][7] = 0;
            board_matrix[5][7] = -4;
            temp = -8;
        }
    }
    else if(board_matrix[x1][y1]==1 && temp == 0 && abs(x1 - x0) == 1)
    {
        board_matrix[x1][4] = 0;
        temp = 9;
    }
    else if(board_matrix[x1][y1]==-1 && temp == 0 && abs(x1 - x0) == 1)
    {
        board_matrix[x1][3] = 0;
        temp = -9;
    }

    return temp;
}

void Engine::UndoMove(int piece0, int piece1, int x0, int y0, int x1, int y1)
{
    previous_moves.pop_back(); //temporary
    if(piece1 == 7 || piece1 == -7)
    {
        //Pawn promotion undo
        piece1 = 0;
    }
    else if(piece1 == 8)
    {
        piece1 = 0;
        if(x1 == 2)
        {
            //Far castle
            board_matrix[0][0] = 4;
            board_matrix[3][0] = 0;
        }
        else if(x1 == 6)
        {
            //Far castle
            board_matrix[7][0] = 4;
            board_matrix[5][0] = 0;
        }
    }
    else if(piece1 == -8)
    {
        piece1 = 0;
        if(x1 == 2)
        {
            //Far castle
            board_matrix[0][7] = -4;
            board_matrix[3][7] = 0;
        }
        else if(x1 == 6)
        {
            //Far castle
            board_matrix[7][7] = -4;
            board_matrix[5][7] = 0;
        }
    }
    else if(piece1 == 9)
    {
        piece1 = 0;
        board_matrix[x1][4] = -1;
    }
    else if(piece1 == -9)
    {
        piece1 = 0;
        board_matrix[x1][3] = 1;
    }
    board_matrix[x0][y0] = piece0;
    board_matrix[x1][y1] = piece1;

}

moves Engine::GetAIMove()
{
    return prev_ai_move;
}



