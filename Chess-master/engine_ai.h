#ifndef _ENGINE_AI_H
#define _ENGINE_AI_H

#include "engine.h"
#include "evaluation.h"


int piece_values1[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 5, 5, 9, 9, 200};
int piece_values2[] = { 0, -1, -1, -1, -1, -1, -1, -1, -1, -3, -3, -3, -3, -5, -5, -9, -9, -200};


int Engine::maximize(int depth)
{
    //EvaluateAllPlayer2Raw();
    //RemoveIllegalMoves2();

    if(depth>0)
    {

        EvaluateAllPlayer2Raw();
        RemoveIllegalMoves2();

        int current_game_state = EvaluateGameState2();
        if(current_game_state == CHECKMATE_PLAYER2)
            return -200;
        else if(current_game_state == STALEMATE)
            return 0;
        else
        {
            int max_value = -100000;

            vector <pair<int,int> > *possible_temp = new vector <pair<int,int> >[17];
            for(int i=1;i<=16;i++)
                possible_temp[i] = player2_possible[i];
            for(int i=1;i<=16;i++)
            {
                for(vector<pair<int,int> >::iterator it = possible_temp[i].begin(); it!= possible_temp[i].end(); ++it)
                {
                    if(player2_current[i].first<0)
                        continue;

                    int previous_size = previous_moves.size();
                    int piece_removed = MakeMove2(player2_current[i].first, player2_current[i].second, it->first, it->second);
                    int current_value = piece_values1[piece_removed] + minimize(depth-1);

                    if(current_value > max_value)
                        max_value = current_value;

                    backtrack_moves(previous_size);
                }
            }

            delete[] possible_temp;
            return max_value;
        }
    }
    return 0;
}

int Engine::minimize(int depth)
{
    if(depth>0)
    {
        EvaluateAllPlayer1Raw();
        RemoveIllegalMoves1();

        int current_game_state = EvaluateGameState1();
        if(current_game_state == CHECKMATE_PLAYER1)
            return 200;
        else if(current_game_state == STALEMATE)
            return 0;
        else
        {
            int j=1;
            int min_value = 100000;

            vector <pair<int,int> > *possible_temp = new vector <pair<int,int> >[17];
            for(int i=1;i<=16;i++)
                possible_temp[i] = player1_possible[i];

            for(int i=1;i<=16;i++)
            {
                for(vector<pair<int,int> >::iterator it = possible_temp[i].begin(); it!= possible_temp[i].end(); ++it)
                {

                    if(player1_current[i].first<0)
                        continue;



                    int previous_size = previous_moves.size();
                    int piece_removed = MakeMove1(player1_current[i].first, player1_current[i].second, it->first, it->second);
                    int current_value = piece_values2[abs(piece_removed)] + maximize(depth-1);

                    if(current_value < min_value)
                        min_value = current_value;

                    backtrack_moves(previous_size);


                }
            }
            delete[] possible_temp;
            return min_value;
        }

    }
    return 0;
}

moves Engine::minimax_base(int depth)
{
    //EvaluateAllPlayer2Raw();
    //RemoveIllegalMoves2();

    int max_value = -100000;
    moves result;

    vector <pair<int,int> > *possible_temp = new vector <pair<int,int> >[17];
    for(int i=1;i<=16;i++)
        possible_temp[i] = player2_possible[i];

    for(int i=1;i<=16;i++)
    {
        for(vector<pair<int,int> >::iterator it = possible_temp[i].begin(); it!= possible_temp[i].end(); ++it)
        {

             if(player2_current[i].first<0)
                continue;




            int previous_size = previous_moves.size();
            int x0 = player2_current[i].first;
            int y0 = player2_current[i].second;
            int x1 = it->first;
            int y1 = it->second;


            int piece_removed = MakeMove2(x0, y0, x1, y1);

            int current_value = piece_values1[piece_removed] + minimize(depth-1);


            backtrack_moves(previous_size);


            if(current_value > max_value)
            {
                result.x0 = x0;
                result.y0 = y0;
                result.x1 = x1;
                result.y1 = y1;
                max_value = current_value;
            }


        }

    }

    EvaluateAllPlayer2Raw();
    RemoveIllegalMoves2();
    delete[] possible_temp;
    return result;

}

void Engine::backtrack_moves(int previous_size)
{
    moves temp;
    while(previous_moves.size()!= previous_size)
    {
        temp = previous_moves.back();
        int index = temp.piece_id;
        int x0 = temp.x0;
        int y0 = temp.y0;
        int x1 = temp.x1;
        int y1 = temp.y1;
        if(x1>=0 && y1>=0)
            board_matrix[x1 + y1*8] = 0;
        board_matrix[x0 + y0*8] = index;
        if(index>0)
            player1_current[index] = make_pair(x0, y0);
        else if(index<0)
            player2_current[-index] = make_pair(x0, y0);
        previous_moves.pop_back();
    }
}





#endif _ENGINE_AI_H
