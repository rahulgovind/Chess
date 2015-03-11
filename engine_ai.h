#include "engine.h"

float values1[] = { 0, 1, 3, 3, 5, 10, 1000};
float values2[] = { 0, -1, -3, -3, -5, -10, -1000};

float quies = 0.25;

int pieces_lost1[7];
int pieces_lost2[7];

float Engine::EvaluateFunction()
{
    float result = 0;
    for(int i=1;i<=6;i++)
        result += values1[i]*pieces_lost1[i] + values2[i]*pieces_lost2[i];

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
            float max_here = -1000000;


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
            float min_here = 1000000;
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

moves Engine::minimax_base(int depth,float alpha, float beta, int no_prune)
{
    for(int i=0;i<7;i++)
    {
        pieces_lost1[i] = 0;
        pieces_lost2[i] = 0;
    }

    moves result;
    if (!IsCheckmate2() && !IsStalemate1())
    {
        float max_here = -1000000;
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

                                float value_evaluated = minimize(depth-1, alpha, beta, no_prune-1);

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
                                board_matrix[x0][y0] = prev_0;
                                board_matrix[x1][y1] = prev_1;
                            }
        return result;
    }
    else
        return moves(0,0,0,0);
}
