#include <cstdio>

#ifndef _ENGINE_H
#define _ENGINE_H

#define GAME_NORMAL 9
#define GAME_CHECKMATE 10
#define GAME_STALEMATE 11
#define GAME_CHECK 12

int initial_board[8][8] ={{ 4,  1,  0,  0,  0,  0,  -1, -4 },
                          { 3,  1,  0,  0,  0,  0,  -1, -3 },
                          { 2,  1,  0,  0,  0,  0,  -1, -2 },
                          { 5,  1,  0,  0,  0,  0,  -1, -5 },
                          { 6,  1,  0,  0,  0,  0,  -1, -6 },
                          { 2,  1,  0,  0,  0,  0,  -1, -2 },
                          { 3,  1,  0,  0,  0,  0,  -1, -3 },
                          { 4,  1,  0,  0,  0,  0,  -1, -4 } };


struct moves
{
    int x0;
    int y0;
    int x1;
    int y1;
    moves() {};
    moves(int X0, int Y0, int X1, int Y1) { x0 = X0; y0 = Y0; x1 = X1; y1 = Y1;}
};
class Engine
{
    int board_matrix[8][8];
    int game_status;
    bool player1;
    bool IsValidMoveBase(int x0, int y0, int x1, int y1);
    bool IsValidPawn1(int x0, int y0, int x1, int y1);
    bool IsValidPawn2(int x0, int y0, int x1, int y1);
    bool IsValidBishop1(int x0, int y0, int x1, int y1);
    bool IsValidBishop2(int x0, int y0, int x1, int y1);
    bool IsValidKnight1(int,int,int,int);
    bool IsValidKnight2(int,int,int,int);
    bool IsValidRook1(int,int,int,int);
    bool IsValidRook2(int,int,int,int);
    bool IsValidQueen1(int,int,int,int);
    bool IsValidQueen2(int,int,int,int);
    bool IsValidKing1(int,int,int,int);
    bool IsValidKing2(int,int,int,int);

    bool IsIllegal1(int,int,int,int);
    bool IsIllegal2(int,int,int,int);
    bool IsCheckmate1();
    bool IsCheckmate2();
    bool IsStalemate1();
    bool IsStalemate2();
    bool IsCheck1();
    bool IsCheck2();

    int MakeMove(int x0, int y0, int x1, int y1);

    moves minimax_base(int depth, float alpha, float beta, int no_prune);
    float minimize(int depth,float alpha, float beta, int no_prune);
    float maximize(int depth,float alpha, float beta, int no_prune);
    int CountPossibleMoves(bool);
    float EvaluateFunction();

public:
    Engine();
    int GetGameStatus();
    int GetCurrentPlayer();
    int GetPiece(int x, int y);
    void ProcessInput(int x0, int y0, int x1, int y1);
    bool IsValidMove(int x0, int y0, int x1, int y1);
};

Engine::Engine()
{

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board_matrix[i][j] = initial_board[i][j];
    player1 = true;
    game_status = GAME_NORMAL;

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
            if(player1==false)
            {
                printf("Thinking. Please wait.");
                moves ai = minimax_base(4, -100000000, 100000000, 0);
                printf(" Move played: %c%c to %c%c\n", -ai.x0+'a', ai.y0+'1', ai.x1+'a', ai.y1+'1');

                ProcessInput(ai.x0, ai.y0, ai.x1, ai.y1);
            }
        }
}

int Engine::MakeMove(int x0, int y0, int x1, int y1)
{
    int temp = board_matrix[x1][y1];
    board_matrix[x1][y1] = board_matrix[x0][y0];
    board_matrix[x0][y0] = 0;

    return temp;
}
#endif // _ENGINE_H
