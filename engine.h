#ifndef _ENGINE_H
#define _ENGINE_H

#include <bits/stdc++.h>


using namespace std;

//constants

#define PLAY_PLAYER1 1
#define PLAY_PLAYER2 2
#define CHECK_PLAYER1 3
#define CHECK_PLAYER2 4
#define CHECKMATE_PLAYER1 5
#define CHECKMATE_PLAYER2 6
#define STALEMATE 7

//Game on = 1
//Checkmate to player 1 = 2
//Checkmate to player 2 = 3
//Stalemate = 4

int initial_position1[][2] = {  {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1},
                                {2, 0}, {5, 0}, {1, 0}, {6, 0}, {0, 0}, {7, 0}, {3, 0}, {4, 0}};

int initial_position2[][2] = {   {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {7, 6},
                                {2, 7}, {5, 7}, {1, 7}, {6, 7}, {0, 7}, {7, 7}, {3, 7}, {4, 7}};



int map_vector_to_board1[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6 };
int map_vector_to_board2[] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -3, -3, -4, -4, -5, -6};

class Engine
{
    private:
    int* board_matrix;
    int* selection_matrix;

    bool selected = false;
    pair<int,int> selected_block;

    pair<int,int> *player1_current;
    pair<int,int> *player2_current;

    vector<pair<int,int> > *player1_possible;
    vector<pair<int,int> > *player2_possible;

    void EvaluateAllPlayer1();
    void EvaluateAllPlayer2();
    void EvaluateAllPlayer1Raw();
    void EvaluateAllPlayer2Raw();

    void EvaluatePawns1();
    void EvaluatePawns2();
    void EvaluateBishops1();
    void EvaluateBishops2();
    void EvaluateKnights1();
    void EvaluateKnights2();
    void EvaluateRooks1();
    void EvaluateRooks2();
    void EvaluateQueen1();
    void EvaluateQueen2();
    void EvaluateKing1();
    void EvaluateKing2();

    bool CheckIfCheck1();
    bool CheckIfCheck2();
    void RemoveIllegalMoves1();
    void RemoveIllegalMoves2();
    bool updated = false;

    int player1 = true;
    void UpdateBoardMatrix();
    void MakeMove(int x0, int y0, int x1, int y1);

    int game_status = 1;

    public:
    Engine();
    int GetPiece(int x,int y);
    int GetColor(int x,int y);
    void ProcessInput(int x,int y);
    bool CheckIfUpdated();
    int GetGameStatus();

    ~Engine();
};

Engine::Engine()
{
    board_matrix = new int[64];
    for(int i=0;i<64;i++)
        board_matrix[i] = 0;

    selection_matrix = new int[64];
    for(int i=0;i<64;i++)
        selection_matrix[i] = 0;

    player1_current = new pair<int,int>[17];
    player2_current = new pair<int,int>[17];


    player1_possible = new vector<pair<int,int> >[17];
    player2_possible = new vector<pair<int,int> >[17];


    for(int i=0;i<16;i++)
    {
        player1_current[i+1] = make_pair(initial_position1[i][0], initial_position1[i][1]);
        player2_current[i+1] = make_pair(initial_position2[i][0], initial_position2[i][1]);
    }

    for(int i=1;i<=16;i++)
    {
        player1_possible[i].clear();
        player2_possible[i].clear();
    }

    UpdateBoardMatrix();


    //Load evaluation for player 1
    EvaluateAllPlayer1();
}


void Engine::UpdateBoardMatrix()
{
    for(int i=0;i<64;i++)
        board_matrix[i] = 0;

    for(int i=1;i<=16;i++)
    {

        int x = player1_current[i].first;
        int y = player1_current[i].second;
        if(x>=0 && y>=0)
            board_matrix[x + y*8] = i;

        x = player2_current[i].first;
        y = player2_current[i].second;
        if(x>=0 && y>=0)
            board_matrix[x + y*8] = -i;
    }
}
int Engine::GetPiece(int x,int y)
{
    int p = board_matrix[x + y*8];
    if(p>=0)
        return map_vector_to_board1[p];
    else if(p<0)
        return map_vector_to_board2[-p];
}

int Engine::GetColor(int x,int y)
{
    return selection_matrix[x + y*8];
}

void Engine::ProcessInput(int x,int y)
{
    if(selected)
    {
        if (x==selected_block.first && y==selected_block.second)
        {
            for(int i=0;i<64;i++)
                selection_matrix[i] = 0;
            selected=false;

            updated = true;
        }
        else
        {
            bool valid_move = false;
            if(player1)
            {
                int index = board_matrix[selected_block.first + (selected_block.second)*8];

                for(vector<pair<int,int> >::iterator it = player1_possible[index].begin();it!=player1_possible[index].end();++it)
                    if(it->first == x && it->second == y)
                    {
                        valid_move = true;
                        MakeMove(selected_block.first,selected_block.second, x, y);
                        break;
                    }
            }
            else
            {
                int index = -board_matrix[selected_block.first + (selected_block.second)*8];

                for(vector<pair<int,int> >::iterator it = player2_possible[index].begin();it!=player2_possible[index].end();++it)
                    if(it->first == x && it->second == y)
                    {
                        valid_move = true;
                        MakeMove(selected_block.first,selected_block.second, x, y);
                        break;
                    }
            }
            if(valid_move)
            {
                for(int i=0;i<64;i++)
                selection_matrix[i] = 0;
                selected=false;
                updated = true;
            }
        }

    }
    else
    {
        if((player1 && board_matrix[x + y*8] > 0) || (!player1 && board_matrix[x+y*8]<0))
        {
            selected_block = make_pair(x, y);
            selected=true;
            selection_matrix[x + y*8] = 1;
            updated=true;

            if(player1)
            {
                int index = board_matrix[x + y*8];
                for(vector<pair<int,int> >::iterator it = player1_possible[index].begin();it!=player1_possible[index].end();++it)
                {
                    selection_matrix[(it->first) + (it->second)*8] = 2;
                }
            }
            else
            {
                int index = -board_matrix[x + y*8];
                for(vector<pair<int,int> >::iterator it = player2_possible[index].begin();it!=player2_possible[index].end();++it)
                {
                    selection_matrix[(it->first) + (it->second)*8] = 2;
                }
            }
        }
    }

}

bool Engine::CheckIfUpdated()
{
    if(updated)
    {
        updated = false;
        return true;
    }
    else
        return false;
}

Engine::~Engine()
{
    delete[] board_matrix;
}

void Engine::MakeMove(int x0, int y0, int x1, int y1)
{
    if(player1)
    {
        int index = board_matrix[x0 + y0*8];
        int index2 = -board_matrix[x1 + y1*8];
        if(index2>0)
            player2_current[index2] = make_pair(-1, -1);
        board_matrix[x0 + y0*8] = 0;
        board_matrix[x1 + y1*8] = index;

        player1_current[index] = make_pair(x1, y1);

        player1 = false;
        EvaluateAllPlayer2();
    }
    else
    {
        int index = -board_matrix[x0 + y0*8];
        int index2 = board_matrix[x1 + y1*8];
        if(index2>0)
            player1_current[index2] = make_pair(-1, -1);
        board_matrix[x0 + y0*8] = 0;
        board_matrix[x1 + y1*8] = -index;

        player2_current[index] = make_pair(x1, y1);

        player1 = true;
        EvaluateAllPlayer1();
    }
}

int Engine::GetGameStatus()
{
    return game_status;
}
#endif _ENGINE_H
