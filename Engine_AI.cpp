#include "engine_ai.h"

const int values[] = { 0, 100, 300, 300, 500, 900, 100000};
const int mobility_value = 5;


moves EngineAI::GetBestMove(int level)
{
    total_moves = 0;
    moves result;
    this->level = level;

    //Decide game stage
    if(previous_moves.size()<20)
    {

        stage = AI_OPENING;
    }
    else if(previous_moves.size()>35)
    {
        stage = AI_END_GAME;
    }
    else
    {
        stage = AI_MIDDLE_GAME;
    }
    //Game difficulty
    switch(level)
    {
    case AI_EASY:
        result = minimax_base(3);
        break;
    case AI_MEDIUM:
        result = minimax_base(4);
        break;
    case AI_HARD:
        result = minimax_base(5);
        break;
    }
    return result;
}

void EngineAI::PrintInfo()
{
    printf("AI level: ");
    switch(level)
    {
    case AI_EASY:
        printf("Easy\n");
        break;
    case AI_MEDIUM:
        printf("Medium\n");
        break;
    case AI_HARD:
        printf("Hard\n");
        break;
    }
    printf("Total leaves evaluated: %u\n", total_moves);

}
int EngineAI::EvaluateOpening()
{

    int result = 0;

    //Relative piece values
    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);


    //Add castling bonus
    if(castled1)
        result-=50;
    if(castled2)
        result+=50;
    //result += mobility_value*(CountPossibleMoves(false) - CountPossibleMoves(true));

    return result;
}

int EngineAI::EvaluateMiddleGame()
{

    int result = 0;
    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);


    //Add castling bonus
    if(castled1)
        result-=50;
    if(castled2)
        result+=50;
    //result += mobility_value*(CountPossibleMoves(false) - CountPossibleMoves(true));

    return result;
}

int EngineAI::EvaluateEndGame()
{

    int result = 0;
    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);


    //Add castling bonus
    if(castled1)
        result-=50;
    if(castled2)
        result+=50;
    //result += mobility_value*(CountPossibleMoves(false) - CountPossibleMoves(true));

    return result;
}

int EngineAI::EvaluateFunction()
{
    total_moves++;
    switch(stage)
    {
    case AI_OPENING:
        return EvaluateOpening();
    case AI_MIDDLE_GAME:
        return EvaluateMiddleGame();
    case AI_END_GAME:
        return EvaluateEndGame();
    }
}

int EngineAI::CountPossibleMoves(bool player1)
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

int EngineAI::maximize(int depth, int max_depth, int alpha, int beta)
{
    if(depth<=max_depth)
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
            int max_here = -100000000;

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

                                    if(prev_1 == -7)
                                        pieces_lost1[5]++;
                                    else if(prev_1 == -8)
                                        castled2 = true;
                                    else if(prev_1 == -9)
                                        pieces_lost1[1]++;
                                    else
                                        pieces_lost1[prev_1]++;

                                    int value_evaluated = minimize(depth+1, max_depth, alpha, beta);

                                    max_here = max(value_evaluated, max_here);
                                    alpha = max(max_here, alpha);

                                    //Undo move
                                    if(prev_1 == -7)
                                        pieces_lost1[5]--;
                                    else if(prev_1 == -8)
                                        castled2 = false;
                                    else if(prev_1 == -9)
                                        pieces_lost1[1]--;
                                    else
                                        pieces_lost1[prev_1]--;
                                    UndoMove(prev_0, prev_1, x0, y0, x1, y1);

                                    if(beta<=alpha)
                                            return max_here;

                                }
            return max_here;
        }
    }

    return EvaluateFunction();
}

int EngineAI::minimize(int depth, int max_depth, int alpha, int beta)
{

    if(depth<=max_depth)
    {
        if(IsCheckmate1())
        {
            pieces_lost1[6]++;
            int result = EvaluateFunction();
            pieces_lost1[6]--;
            return result;
        }
        else if(IsStalemate1())
            return EvaluateFunction();
        else
        {
            int min_here = 100000000;
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
                                    else if(prev_1 == 8)
                                        castled1 = true;
                                    else if(prev_1 == 9)
                                        pieces_lost2[1]++;
                                    else
                                        pieces_lost2[-prev_1]++;

                                    int value_evaluated = maximize(depth+1, max_depth, alpha, beta);


                                    min_here = min(value_evaluated, min_here);
                                    beta = min(min_here, beta);

                                    //Undo move
                                    if(prev_1 == 7)
                                        pieces_lost2[5]--;
                                    else if(prev_1 == 8)
                                        castled1 = false;
                                    else if(prev_1 == 9)
                                        pieces_lost2[1]--;
                                    else
                                        pieces_lost2[-prev_1]--;
                                    UndoMove(prev_0, prev_1, x0, y0, x1, y1);


                                    if(beta<=alpha)
                                        return min_here;

                                }
            return min_here;
        }
    }
    return EvaluateFunction();
}

moves EngineAI::minimax_base(int max_depth, int alpha, int beta)
{
    for(int i=0;i<7;i++)
    {
        pieces_lost1[i] = 0;
        pieces_lost2[i] = 0;
    }

    moves result{0,0,0,0};
    if (!(IsCheckmate2()) && !(IsStalemate1()))
    {
        int max_here = -100000000;
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

                                if(prev_1 == -7)
                                    pieces_lost1[5]++;
                                else if(prev_1 == -8)
                                    castled2 = true;
                                else if(prev_1 == -9)
                                    pieces_lost1[1]++;
                                else
                                    pieces_lost1[prev_1]++;

                                int value_evaluated = minimize(2, max_depth, alpha, beta);

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
                                if(prev_1 == -7)
                                    pieces_lost1[5]--;
                                else if(prev_1 == -8)
                                    castled2 = false;
                                else if(prev_1 == -9)
                                    pieces_lost1[1]--;
                                else
                                    pieces_lost1[prev_1]--;

                                UndoMove(prev_0, prev_1, x0, y0, x1, y1);
                            }
            }
    }

    return result;
}


