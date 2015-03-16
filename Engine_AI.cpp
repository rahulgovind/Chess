#include "engine.h"
#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>

//Killer moves heuristic removed
using namespace std;

float values[] = { 0, 100, 300, 300, 500, 900, 100000};
float mobility_value = 5;
bool set_quies = true;
int pieces_lost1[7];
int pieces_lost2[7];
float quies_factor = 0.25;
bool first_move_played = false;
int total_count = 0;

float Engine::EvaluateFunction()
{
    float result = 0;

    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);

    //Just for debugging
    total_count++;

    result += mobility_value*(CountPossibleMoves(false) - CountPossibleMoves(true));

    return result;
}

int Engine::CountPossibleMoves(bool player1)
{
    int result = 0;
     for(int x0=0;x0<8;x0++)
        for(int y0=0;y0<8;y0++)
        {
            if((player1 && board_matrix[x0][y0]>0) || (!player1 && board_matrix[x0][y0]<0))
            {
            for(int x1=0;x1<8;x1++)
                for(int y1=0;y1<8;y1++)
                    if(IsValidMoveBase(x0, y0, x1, y1))
                        result++;
            }
        }
    return result;

}

float Engine::maximize(int depth, int max_depth, float alpha, float beta, int extra)
{
    if(depth<=max_depth+extra)
    {
        if(IsCheckmate2())
        {
            pieces_lost2[6]++;
            float result = EvaluateFunction();
            pieces_lost2[6]--;

            return result;
        }
        else if(IsStalemate2())
        {
            return EvaluateFunction();
        }
        else
        {
            float max_here = -100000000;

            //Normal minimax with alpha - beta pruning
            for(int x0=0;x0<8;x0++)
                for(int y0=0;y0<8;y0++)
                    for(int x1=0;x1<8;x1++)
                        for(int y1=0;y1<8;y1++)
                            if(board_matrix[x0][y0]<0)
                                if(IsValidMove(x0,y0,x1,y1))
                                {
                                    //Do move
                                    int prev_0 = board_matrix[x0][y0];
                                    int prev_1 = MakeMove(x0, y0, x1, y1);

                                    pieces_lost1[prev_1]++;

                                    float value_evaluated = minimize(depth+1, max_depth, alpha, beta, extra);

                                    max_here = max(value_evaluated, max_here);
                                    alpha = max(max_here, alpha);

                                    //Undo move
                                    pieces_lost1[prev_1]--;
                                    UndoMove(prev_0, prev_1, x0, y0, x1, y1);

                                    if(beta<=alpha)
                                            return max_here;

                                }
            return max_here;
        }
    }

    //Count total pieces lost
    int total_pieces_lost = 0;
    for(int i=1;i<=6;i++)
        total_pieces_lost+=pieces_lost1[i] + pieces_lost2[i];
    extra = (int)(quies_factor*total_pieces_lost);
    if(depth<=max_depth+extra && set_quies)
        return maximize(depth, max_depth, alpha, beta, extra);

    return EvaluateFunction();
}

float Engine::minimize(int depth, int max_depth, float alpha, float beta, int extra)
{

    if(depth<=max_depth+extra)
    {
        if(IsCheckmate1())
        {
            pieces_lost1[6]++;
            float result = EvaluateFunction();
            pieces_lost1[6]--;
            return result;
        }
        else if(IsStalemate1())
            return EvaluateFunction();
        else
        {
            float min_here = 100000000;
            for(int x0=0;x0<8;x0++)
                for(int y0=0;y0<8;y0++)
                    for(int x1=0;x1<8;x1++)
                        for(int y1=0;y1<8;y1++)
                            if(board_matrix[x0][y0]>0)
                                if(IsValidMove(x0,y0,x1,y1))
                                {
                                      //Do move
                                    int prev_0 = board_matrix[x0][y0];
                                    int prev_1 = MakeMove(x0, y0, x1, y1);
                                    if(prev_1 == 7)
                                        pieces_lost2[5]++;
                                    else
                                        pieces_lost2[-prev_1]++;

                                    float value_evaluated = maximize(depth+1, max_depth, alpha, beta, extra);


                                    min_here = min(value_evaluated, min_here);
                                    beta = min(min_here, beta);

                                    //Undo move
                                    pieces_lost2[-prev_1]--;
                                    UndoMove(prev_0, prev_1, x0, y0, x1, y1);


                                    if(beta<=alpha)
                                        return min_here;

                                }
            return min_here;
        }
    }

    //Count total pieces lost
    int total_pieces_lost = 0;
    for(int i=1;i<=6;i++)
        total_pieces_lost+=pieces_lost1[i] + pieces_lost2[i];
    extra = (int)(quies_factor*total_pieces_lost);
    if(depth<=max_depth+extra && set_quies)
        return minimize(depth, max_depth, alpha, beta, extra);
    return EvaluateFunction();
}

moves Engine::minimax_base(int max_depth, float alpha, float beta)
{
    for(int i=0;i<7;i++)
    {
        pieces_lost1[i] = 0;
        pieces_lost2[i] = 0;
    }

    moves result;
    if (!(IsCheckmate2()) && !(IsStalemate1()))
    {
        float max_here = -100000000;
        for(int x0=0;x0<8;x0++)
            for(int y0=0;y0<8;y0++)
            {
                for(int x1=0;x1<8;x1++)
                    for(int y1=0;y1<8;y1++)
                        if(board_matrix[x0][y0]<0)
                            if(IsValidMove(x0,y0,x1,y1))
                            {
                                //Do move
                                int prev_0 = board_matrix[x0][y0];
                                int prev_1 = MakeMove(x0, y0, x1, y1);

                                pieces_lost1[prev_1]++;

                                float value_evaluated = minimize(2, max_depth, alpha, beta, 0);

                                if(value_evaluated > max_here)
                                {
                                    max_here = value_evaluated;
                                    result.x0 = x0;
                                    result.y0 = y0;
                                    result.x1 = x1;
                                    result.y1 = y1;
                                }
                                alpha = max(max_here, alpha);

                                //Undo move
                                pieces_lost1[prev_1]--;

                                UndoMove(prev_0, prev_1, x0, y0, x1, y1);
                            }
            }
        return result;
    }
    else
        return moves(0,0,0,0);
}

long unsigned int __stdcall Engine::AIThread(void *input)
{
    Engine *main_engine = (Engine*)input;
    Engine *test_engine = new Engine(input);
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            test_engine->board_matrix[i][j] = main_engine->board_matrix[i][j];
    printf("Thinking. \n");

    moves ai;
    total_count = 0;
    ai = test_engine->minimax_base(4, -100000000, 100000000);

    printf("Total leaves evaluated: %u\n\n", total_count);
    main_engine->prev_ai_move = moves(ai.x0, ai.y0, ai.x1, ai.y1);
    main_engine->ProcessInput(ai.x0, ai.y0, ai.x1, ai.y1);

    return 0;
}

void Engine::MakeAIMove()
{
    //AIThread(this);
    CreateThread(NULL, 0, AIThread, (void*)this, 0, 0);
}
