#include "engine_ai.h"
#include <windows.h>
#include <ctime>

//Evaluation points
//for material advantage

const int values[] = { 0, 100, 300, 300, 500, 900, 100000};

//Different evaluation bonus points for
//Positional advantage

const int mobility_value = 3;
const int castle_value = 60;
const int early_queen = 20;
const int bishop_long_diagonal = 30;
const int developed_knight = 35;
const int double_bishop = 45;
const int doubled_pawn = 10;
const int pawn_forward_bonus = 5;

moves EngineAI::GetBestMove(int level)
{
    total_moves = 0;
    quies_extra_max = 0;
    moves result;
    this->level = level;
    int max_time;
    bool first_modified = false;
    //Game difficulty
    switch(level)
    {
    case AI_EASY:
        max_time = 1;
        break;
    case AI_MEDIUM:
        max_time = 4;
        break;
    case AI_HARD:
        max_time = 10;
        break;
    }


    //Decide game stage
    if(previous_moves.size()<20)
    {
        quies_factor = 0.5;
        stage = AI_OPENING;
    }
    else if(previous_moves.size()>50)
    {
        quies_factor = 0.25;
        stage = AI_END_GAME;
    }
    else
    {
        quies_factor = 0.25;
        stage = AI_MIDDLE_GAME;
    }

    //Create thread to look for best move
    HANDLE hThread = CreateThread(NULL, 0, AIThread, (void*)this, 0, 0);

    modified = false;
    max_d = 1;


    int init_time = time(NULL);
    while(true)
    {
        Sleep(5);

        if(modified)
        {
            first_modified=true;
            modified = false;
            max_d++;
            hThread = CreateThread(NULL, 0, AIThread, (void*)this, 0, 0);
        }
        if(first_modified && (time(NULL)-init_time>max_time))
        {
            TerminateThread(hThread, 0);
            break;
        }
    }

    return best_move;
}

long unsigned int __stdcall EngineAI::AIThread(void *input)
{
    //Create EngineAI object to look for best move
    EngineAI *engine_ai = (EngineAI*)input;
    engine_ai->best_move = engine_ai->minimax_base(engine_ai->max_d);
    engine_ai->modified = true;
    return 0;
}

void EngineAI::PrintInfo()
{
    //Print AI Level
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
    //Print game stage
    printf("Game stage: ");
    switch(stage)
    {
    case AI_OPENING:
        printf("Opening\n");
        break;
    case AI_MIDDLE_GAME:
        printf("Middle game\n");
        break;
    case AI_END_GAME:
        printf("End game\n");
        break;
    }

    //Print other AI statistics
    printf("Base max depth: %u\n", (modified)? max_d-1 : max_d);
    printf("Total leaves evaluated: %u\n", total_moves);
    printf("Maximum extra moves for quiescence: %u\n", quies_extra_max);

    printf("\n");

}
int EngineAI::EvaluateOpening()
{

    int result = 0;

    //Relative piece values
    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);


    //Add castling bonus
    if(castled1)
        result-=castle_value;
    if(castled2)
        result+=castle_value;

    //mobility
    result += mobility_value*(CountPossibleMoves(false) - CountPossibleMoves(true));

    //Early Queen
    if(board_matrix[3][0]!=5)
        result+=early_queen;
    if(board_matrix[3][7]!=-5)
        result-=early_queen;

    //Doubled pawn
    for(int x=0;x<8;x++)
    {
        bool white_pawn = false;
        bool black_pawn = false;
        for(int y=0;y<8;y++)
        {
            if(board_matrix[x][y]==1)
            {
                if(white_pawn)
                    result+=doubled_pawn;
                else
                    white_pawn=true;
            }
            else if(board_matrix[x][y]==-1)
            {
                if(black_pawn)
                    result-=doubled_pawn;
                else
                    black_pawn=true;
            }
        }
    }

    //Developed knight
    if(board_matrix[1][0]!=3)
        result-=developed_knight;
    if(board_matrix[6][0]!=3)
        result-=developed_knight;
    if(board_matrix[1][7]!=-3)
        result+=developed_knight;
    if(board_matrix[6][7]!=-3)
        result+=developed_knight;

    return result;
}

int EngineAI::EvaluateMiddleGame()
{

    int result = 0;
    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);

    //Double bishop
    if(pieces_lost1[2]==1)
        result+=double_bishop;
    if(pieces_lost2[2]==1)
        result-=double_bishop;

    //Doubled pawn
    for(int x=0;x<8;x++)
    {
        bool white_pawn = false;
        bool black_pawn = false;
        for(int y=0;y<8;y++)
        {
            if(board_matrix[x][y]==1)
            {
                if(white_pawn)
                    result+=doubled_pawn;
                else
                    white_pawn=true;
            }
            else if(board_matrix[x][y]==-1)
            {
                if(black_pawn)
                    result-=doubled_pawn;
                else
                    black_pawn=true;
            }
        }
    }


    return result;
}

int EngineAI::EvaluateEndGame()
{

    int result = 0;
    for(int i=1;i<=6;i++)
        result += values[i]*(pieces_lost1[i] - pieces_lost2[i]);

    //Doubled pawn
    for(int x=0;x<8;x++)
    {
        bool white_pawn = false;
        bool black_pawn = false;
        for(int y=0;y<8;y++)
        {
            if(board_matrix[x][y]==1)
            {
                if(white_pawn)
                    result+=doubled_pawn;
                else
                    white_pawn=true;
            }
            else if(board_matrix[x][y]==-1)
            {
                if(black_pawn)
                    result-=doubled_pawn;
                else
                    black_pawn=true;
            }
        }
    }

    //Favor pawn movement
    for(int x=0;x<8;x++)
        for(int y=0;y<8;y++)
        {
            if(board_matrix[x][y]==1)
                result-=pawn_forward_bonus*(y);
            if(board_matrix[x][y]==-1)
                result+=pawn_forward_bonus*(8-y);
        }
    return result;
}

//Main evaluation function
//Calls appropriate evaluation function based
//on game stage

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

//Counts the total number of possible moves
//for a given game state

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

//Minimax part for maximizing player
int EngineAI::maximize(int depth, int max_depth, int alpha, int beta, int extra)
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

                                    if(prev_1 >= 9)
                                    {
                                        pieces_lost1[5]++;
                                        pieces_lost1[prev_1 - 9]++;
                                    }
                                    else if(prev_1 == -7)
                                        castled2 = true;
                                    else if(prev_1 == -8)
                                        pieces_lost1[1]++;
                                    else
                                        pieces_lost1[prev_1]++;

                                    int value_evaluated = minimize(depth+1, max_depth, alpha, beta, extra);

                                    max_here = max(value_evaluated, max_here);
                                    alpha = max(max_here, alpha);

                                    //Undo move
                                    if(prev_1 >= 9)
                                    {
                                        pieces_lost1[5]--;
                                        pieces_lost1[prev_1-9]--;
                                    }
                                    else if(prev_1 == -7)
                                        castled2 = false;
                                    else if(prev_1 == -8)
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

    //Quiescence search
    int total_pieces_lost = 0;
    for(int i=1;i<=6;i++)
        total_pieces_lost+= pieces_lost1[i] + pieces_lost2[i];

    extra = quies_factor*(float)total_pieces_lost;
    if(depth<=max_depth+extra)
    {
        quies_extra_max = max(extra, quies_extra_max);
        return maximize(depth, max_depth, alpha, beta, extra);
    }
    return EvaluateFunction();
}

//Minimax part for the minimizing player
int EngineAI::minimize(int depth, int max_depth, int alpha, int beta, int extra)
{

    if(depth<=max_depth+extra)
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

                                    if(prev_1 <= -9)
                                    {
                                        pieces_lost2[5]++;
                                        pieces_lost2[-prev_1-9]++;
                                    }
                                    else if(prev_1 == 7)
                                        castled1 = true;
                                    else if(prev_1 == 8)
                                        pieces_lost2[1]++;
                                    else
                                        pieces_lost2[-prev_1]++;

                                    int value_evaluated = maximize(depth+1, max_depth, alpha, beta, extra);


                                    min_here = min(value_evaluated, min_here);
                                    beta = min(min_here, beta);

                                    //Undo move
                                    if(prev_1 <= -9)
                                    {
                                        pieces_lost2[5]--;
                                        pieces_lost2[-prev_1 - 9]--;
                                    }
                                    else if(prev_1 == 7)
                                        castled1 = false;
                                    else if(prev_1 == 8)
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

    //Quiescence
    int total_pieces_lost = 0;
    for(int i=1;i<=6;i++)
        total_pieces_lost+= pieces_lost1[i] + pieces_lost2[i];

    extra = quies_factor*(float)total_pieces_lost;
    if(depth<=max_depth+extra)
    {
        quies_extra_max = max(extra, quies_extra_max);
        return minimize(depth, max_depth, alpha, beta, extra);
    }
    return EvaluateFunction();
}

//Base function for minimax
//Sets appropriate variables
//and then does the maximizing part
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

                                if(prev_1 >= 9)
                                {
                                    pieces_lost1[5]++;
                                    pieces_lost1[prev_1-9]++;
                                }
                                else if(prev_1 == -7)
                                    castled2 = true;
                                else if(prev_1 == -8)
                                    pieces_lost1[1]++;
                                else
                                    pieces_lost1[prev_1]++;

                                int value_evaluated = minimize(2, max_depth, alpha, beta, 0);

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
                                if(prev_1 >= 9)
                                {
                                    pieces_lost1[5]--;
                                    pieces_lost1[prev_1 - 9]--;
                                }
                                else if(prev_1 == -7)
                                    castled2 = false;
                                else if(prev_1 == -8)
                                    pieces_lost1[1]--;
                                else
                                    pieces_lost1[prev_1]--;

                                UndoMove(prev_0, prev_1, x0, y0, x1, y1);

                            }
            }
    }

    return result;
}


