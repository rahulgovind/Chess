#include <cstdio>

#ifndef _ENGINE_H
#define _ENGINE_H

#define GAME_NORMAL 9
#define GAME_CHECKMATE 10
#define GAME_STALEMATE 11
#define GAME_CHECK 12
#define GAME_THINKING 13

#include <windows.h>

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
    void MakeAIMove();
    static moves minimax_base(Engine*,int depth, float alpha, float beta, int no_prune);
    float minimize(int depth, int max_depth,float alpha, float beta, int no_prune);
    float maximize(int depth, int max_depth,float alpha, float beta, int no_prune);
    int CountPossibleMoves(bool);
    float EvaluateFunction();

    bool ai_mode;
    moves prev_ai_move;

    static long unsigned int __stdcall AIThread(void*);
public:
    Engine(bool);
    int GetGameStatus();
    int GetCurrentPlayer();
    int GetPiece(int x, int y);
    void ProcessInput(int x0, int y0, int x1, int y1);
    bool IsValidMove(int x0, int y0, int x1, int y1);
    moves GetAIMove();
};



#endif // _ENGINE_H
