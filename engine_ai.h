#ifndef _ENGINE_AI_H
#define _ENGINE_AI_H

#include "board.h"
#include <cstdio>

#define AI_EASY 1
#define AI_MEDIUM 2
#define AI_HARD 3

#define AI_OPENING 10
#define AI_MIDDLE_GAME 11
#define AI_END_GAME 12


class EngineAI:public Board
{
private:
    int minimize(int depth, int max_depth, int alpha, int beta, int extra);
    int maximize(int depth, int max_depth,int alpha, int beta, int extra);
    moves minimax_base(int depth,int alpha=-100000000, int beta=100000000);
    int CountPossibleMoves(bool);

    int EvaluateOpening();
    int EvaluateMiddleGame();
    int EvaluateEndGame();

    int EvaluateFunction();

    float quies_factor;
    bool castled1 = false;
    bool castled2 = false;

    int pieces_lost1[7];
    int pieces_lost2[7];

    int level;
    int stage;

    int total_moves;
    int quies_extra_max;
public:
    EngineAI(Board board):Board(board){}
    moves GetBestMove(int level = AI_EASY);
    void PrintInfo();

};

#endif //_ENGINE_AI_H
