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
    moves prev_ai_move;

    static long unsigned int __stdcall AIThread(void*);
public:
    Engine(bool);
    Engine(bool, string);
    int GetGameStatus();
    int GetCurrentPlayer();
    int GetPiece(int x, int y);
    void ProcessInput(int x0, int y0, int x1, int y1);
    moves GetAIMove();
    void MakeAIMove();
    bool UndoGame();
};


#endif // _ENGINE_H
