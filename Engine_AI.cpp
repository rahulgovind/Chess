#include "engine.h"
#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

float values[] = { 0, 100, 300, 300, 500, 900, 100000};

float mobility_value = 5;
float quies = 0.25;

int pieces_lost1[7];
int pieces_lost2[7];

list<moves> killer_moves[12];
int killer_moves_max = 3;

float Engine::EvaluateFunction()
{
    float result = 0;

    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);

    result += mobility_value*(CountPossibleMoves(false) - CountPossibleMoves(true));

    return result;
}

int Engine::CountPossibleMoves(bool player1)
{
    int result = 0;
     for(int x0=0;x0<8;x0++)
        for(int y0=0;y0<8;y0++)
            for(int x1=0;x1<8;x1++)
                for(int y1=0;y1<8;y1++)
                    if((player1 && board_matrix[x0][y0]>0) || (!player1 && board_matrix[x0][y0]<0))
                        if(IsValidMoveBase(x0, y0, x1, y1))
                            result++;
    return result;

}

float Engine::maximize(int depth, float alpha, float beta, int no_prune)
{
    if(depth>0)
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
            //Killer move heuristic
            if(killer_moves[depth].size()>0)
            {
                for(list<moves>::iterator it =  killer_moves[depth].begin(); it!= killer_moves[depth].end(); ++it)
                {
                    int x0 = it->x0, y0 = it->y0, x1 = it->x1, y1 = it->y1;
                    if(IsValidMove(x0, y0, x1, y1))
                    {
                         //Do move
                        int prev_0 = board_matrix[x0][y0];
                        int prev_1 = board_matrix[x1][y1];
                        board_matrix[x0][y0]= 0;
                        board_matrix[x1][y1] = prev_0;
                        pieces_lost1[prev_1]++;

                        float value_evaluated = minimize(depth-1, alpha, beta, no_prune - 1);

                        max_here = max(value_evaluated, max_here);
                        alpha = max(max_here, alpha);

                        //Undo move
                        pieces_lost1[prev_1]--;
                        board_matrix[x0][y0] = prev_0;
                        board_matrix[x1][y1] = prev_1;

                        if(beta<=alpha)
                        {
                            if(killer_moves[depth].size()==killer_moves_max)
                                killer_moves[depth].pop_back();
                            killer_moves[depth].emplace_front(x0, y0, x1, y1);

                            if(no_prune<=0)
                                return max_here;
                        }
                    }
                }
            }
            max_here = -100000000;


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
                                    int prev_1 = board_matrix[x1][y1];
                                    board_matrix[x0][y0]= 0;
                                    board_matrix[x1][y1] = prev_0;
                                    pieces_lost1[prev_1]++;

                                    float value_evaluated = minimize(depth-1, alpha, beta, no_prune - 1);

                                    max_here = max(value_evaluated, max_here);
                                    alpha = max(max_here, alpha);

                                    //Undo move
                                    pieces_lost1[prev_1]--;
                                    board_matrix[x0][y0] = prev_0;
                                    board_matrix[x1][y1] = prev_1;

                                    if(beta<=alpha)
                                    {

                                        if(no_prune<=0)
                                            return max_here;
                                    }
                                }
            return max_here;
        }
    }

    return EvaluateFunction();
}


float Engine::minimize(int depth, float alpha, float beta,int no_prune)
{

    if(depth>0)
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

            //Killer move heuristic
            if(killer_moves[depth].size()>0)
            {
                for(list<moves>::iterator it =  killer_moves[depth].begin(); it!= killer_moves[depth].end(); ++it)
                {
                    int x0 = it->x0, y0 = it->y0, x1 = it->x1, y1 = it->y1;
                    if(IsValidMove(x0, y0, x1, y1))
                    {
                        //Do move
                        int prev_0 = board_matrix[x0][y0];
                        int prev_1 = board_matrix[x1][y1];
                        board_matrix[x0][y0]= 0;
                        board_matrix[x1][y1] = prev_0;
                        pieces_lost2[-prev_1]++;

                        float value_evaluated = maximize(depth-1, alpha, beta, no_prune-1);


                        min_here = min(value_evaluated, min_here);
                        beta = min(min_here, beta);

                        //Undo move
                        pieces_lost2[-prev_1]--;
                        board_matrix[x0][y0] = prev_0;
                        board_matrix[x1][y1] = prev_1;


                        if(beta<=alpha)
                        {
                            if(killer_moves[depth].size()==killer_moves_max)
                                killer_moves[depth].pop_back();

                            killer_moves[depth].emplace_front(x0, x1, y0, y1);
                            if(no_prune<=0)
                                return min_here;
                        }
                    }
                }
            }
            min_here = 100000000;
            for(int x0=0;x0<8;x0++)
                for(int y0=0;y0<8;y0++)
                    for(int x1=0;x1<8;x1++)
                        for(int y1=0;y1<8;y1++)
                            if(board_matrix[x0][y0]>0)
                                if(IsValidMove(x0,y0,x1,y1))
                                {
                                      //Do move
                                    int prev_0 = board_matrix[x0][y0];
                                    int prev_1 = board_matrix[x1][y1];
                                    board_matrix[x0][y0]= 0;
                                    board_matrix[x1][y1] = prev_0;
                                    pieces_lost2[-prev_1]++;

                                    float value_evaluated = maximize(depth-1, alpha, beta, no_prune-1);


                                    min_here = min(value_evaluated, min_here);
                                    beta = min(min_here, beta);

                                    //Undo move
                                    pieces_lost2[-prev_1]--;
                                    board_matrix[x0][y0] = prev_0;
                                    board_matrix[x1][y1] = prev_1;


                                    if(beta<=alpha)
                                    {
                                        if(no_prune<=0)
                                            return min_here;
                                    }
                                }
            return min_here;
        }
    }
    return EvaluateFunction();
}

moves Engine::minimax_base(Engine *eng, int depth,float alpha, float beta, int no_prune)
{
    for(int i=0;i<7;i++)
    {
        pieces_lost1[i] = 0;
        pieces_lost2[i] = 0;
    }

    for(int i=0;i<12;i++)
        killer_moves[i].clear();
    moves result;
    if (!(eng->IsCheckmate2()) && !(eng->IsStalemate1()))
    {
        float max_here = -100000000;
        for(int x0=0;x0<8;x0++)
            for(int y0=0;y0<8;y0++)
                for(int x1=0;x1<8;x1++)
                    for(int y1=0;y1<8;y1++)
                        if(eng->board_matrix[x0][y0]<0)
                            if(eng->IsValidMove(x0,y0,x1,y1))
                            {
                                //Do move
                                int prev_0 = eng->board_matrix[x0][y0];
                                int prev_1 = eng->board_matrix[x1][y1];

                                eng->board_matrix[x0][y0]= 0;
                                eng->board_matrix[x1][y1] = prev_0;
                                pieces_lost1[prev_1]++;

                                float value_evaluated = eng->minimize(depth-1, alpha, beta, no_prune-1);

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
                                eng->board_matrix[x0][y0] = prev_0;
                                eng->board_matrix[x1][y1] = prev_1;
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
    printf("Thinking. Please wait.");
    moves ai = Engine::minimax_base(test_engine, 4, -100000000, 100000000, 0);
    printf("Move played: %c%c to %c%c\n", ai.x0+'a', ai.y0+'1', ai.x1+'a', ai.y1+'1');
    main_engine->prev_ai_move = moves(ai.x0, ai.y0, ai.x1, ai.y1);
    main_engine->ProcessInput(ai.x0, ai.y0, ai.x1, ai.y1);
}

void Engine::MakeAIMove()
{
    CreateThread(NULL, 0, AIThread, (void*)this, 0, 0);
}
