#ifndef _ENGINE_H
#define _ENGINE_H

#include "board.h"
#include <windows.h>
#include <cstdio>

#define GAME_NORMAL 9
#define GAME_CHECKMATE 10
#define GAME_STALEMATE 11
#define GAME_CHECK 12
#define GAME_THINKING 13
#define ENGINE_DEBUG true

class Engine: public Board
{
private:
    int game_status;
    bool player1;
    bool ai_mode;
    int ai_difficulty;
    moves prev_ai_move;

    static long unsigned int __stdcall AIThread(void*);
public:
    Engine(bool AI_mode, int difficulty=1);
    Engine(bool, string);
    int GetGameStatus();
    int GetCurrentPlayer();
    int GetPiece(int x, int y);
    void ProcessInput(int x0, int y0, int x1, int y1, int pawn_promo=0);
    moves GetAIMove();
    void MakeAIMove();
    bool UndoGame();
    bool IsPawnPromotion(int,int,int,int);
};


#endif // _ENGINE_H
