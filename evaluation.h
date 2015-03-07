#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "engine.h"

void Engine::EvaluateAllPlayer1()
{
    EvaluateAllPlayer1Raw();

    bool check = CheckIfCheck1();
    RemoveIllegalMoves1();

    int totalsize = 0;
    for(int i=1;i<=16;i++)
        if(player1_current[i].first>=0)
            totalsize+=player1_possible[i].size();

    if(totalsize==0)
        if(check)
            game_status = CHECKMATE_PLAYER1;
        else
            game_status = STALEMATE;
    else if(check)
    {
        game_status = CHECK_PLAYER1;
        printf("Check to player 1\n");
    }
    else
        game_status = PLAY_PLAYER1;
}

void Engine::EvaluateAllPlayer2()
{
    EvaluateAllPlayer2Raw();

    bool check = CheckIfCheck2();

    RemoveIllegalMoves2();

    int totalsize = 0;
    for(int i=1;i<=16;i++)
        if(player2_current[i].first >=0)
            totalsize+=player2_possible[i].size();

    if(totalsize==0)
        if(check)
            game_status = CHECKMATE_PLAYER2;
        else
            game_status = STALEMATE;
    else if(check)
    {
        game_status = CHECK_PLAYER2;
        printf("Check to player 2\n");
    }
    else
        game_status = PLAY_PLAYER2;
}

void Engine::EvaluateAllPlayer1Raw()
{
    EvaluatePawns1();
    EvaluateBishops1();
    EvaluateKnights1();
    EvaluateRooks1();
    EvaluateQueen1();
    EvaluateKing1();
}

void Engine::EvaluateAllPlayer2Raw()
{
    EvaluatePawns2();
    EvaluateBishops2();
    EvaluateKnights2();
    EvaluateRooks2();
    EvaluateQueen2();
    EvaluateKing2();
}

void Engine::RemoveIllegalMoves1()
{
    //Remove moves that would leave the king open to attack

    for(int i=1;i<=16;i++)
    {
        for(vector<pair<int,int> >::iterator it = player1_possible[i].begin();it!=player1_possible[i].end();)
        {
            //Source piece
            int x0 = player1_current[i].first;
            int y0 = player1_current[i].second;

            bool illegal_move = false;
            if(x0>=0 && y0>=0)
            {

                //Remove player1's piece
                board_matrix[x0 + y0*8] = 0;

                //Remove opponent's piece
                int x1 = it->first, y1 = it->second;
                int index2 = -board_matrix[x1 + y1*8];

                bool piece_removed = false;
                if(board_matrix[x1 + y1*8]<0)
                {
                    piece_removed = true;
                    player2_current[index2] = make_pair(-1, -1);
                }
                //Put own piece
                board_matrix[x1 + y1*8] = i;
                player1_current[i] = make_pair(x1, y1);

                //Check if check
                if(CheckIfCheck1())
                    illegal_move = true;

                //Reverse changes
                board_matrix[x1 + y1*8] = -index2;
                if(piece_removed)
                    player2_current[index2] = make_pair(x1, y1);

                board_matrix[x0 + y0*8] = i;
                player1_current[i] = make_pair(x0, y0);
            }

            if(illegal_move)
                it = player1_possible[i].erase(it);
            else
                ++it;
        }
    }
}

void Engine::RemoveIllegalMoves2()
{
    //Remove moves that would leave the king open to attack

    for(int i=1;i<=16;i++)
    {
        for(vector<pair<int,int> >::iterator it = player2_possible[i].begin();it!=player2_possible[i].end();)
        {
            //Source piece
            int x0 = player2_current[i].first;
            int y0 = player2_current[i].second;

            bool illegal_move = false;
            if(x0>=0 && y0>=0)
            {

                //Remove player2's piece
                board_matrix[x0 + y0*8] = 0;

                //Remove opponent's piece
                int x1 = it->first, y1 = it->second;
                int index2 = board_matrix[x1 + y1*8];

                bool piece_removed = false;
                if(board_matrix[x1 + y1*8]>0)
                {
                    piece_removed = true;
                    player1_current[index2] = make_pair(-1, -1);
                }
                //Put own piece
                board_matrix[x1 + y1*8] = -i;
                player2_current[i] = make_pair(x1, y1);

                //Check if check
                if(CheckIfCheck2())
                    illegal_move = true;

                //Reverse changes
                board_matrix[x1 + y1*8] = index2;
                if(piece_removed)
                    player1_current[index2] = make_pair(x1, y1);

                board_matrix[x0 + y0*8] = -i;
                player2_current[i] = make_pair(x0, y0);
            }

            if(illegal_move)
                it = player2_possible[i].erase(it);
            else
                ++it;
        }
    }
}
void Engine::EvaluatePawns1()
{
    for(int i=1;i<=8;i++)
    {
        player1_possible[i].clear();
        int x = player1_current[i].first;
        int y = player1_current[i].second;

        if(x>=0 && y>=0)
        {
            if(y+1<8)
            {
                //Normal move
                if(board_matrix[x + (y+1)*8]==0)
                {
                    player1_possible[i].push_back(make_pair(x, y+1));
                    //Double move
                    if(y==1)
                        if(board_matrix[x+(y+2)*8]==0)
                            player1_possible[i].push_back(make_pair(x, y+2));
                }

                //Crose move to capture
                if(x+1<8)
                    if(board_matrix[(x+1)+(y+1)*8]<0)
                        player1_possible[i].push_back(make_pair(x+1, y+1));
                if(x-1>=0)
                    if(board_matrix[(x-1)+(y+1)*8]<0)
                        player1_possible[i].push_back(make_pair(x-1, y+1));
            }
        }
    }

}

void Engine::EvaluatePawns2()
{
     for(int i=1;i<=8;i++)
    {
        player2_possible[i].clear();
        int x = player2_current[i].first;
        int y = player2_current[i].second;

        if(x>=0 && y>=0)
        {
            if(y-1<8)
            {
                //Normal move
                if(board_matrix[x + (y-1)*8]==0)
                {
                    player2_possible[i].push_back(make_pair(x, y-1));

                    //Double move
                    if(y==6)
                        if(board_matrix[x+(y-2)*8]==0)
                            player2_possible[i].push_back(make_pair(x, y-2));
                }

                //Crose move to capture
                if(x+1<8)
                    if(board_matrix[(x+1)+(y-1)*8]>0)
                        player2_possible[i].push_back(make_pair(x+1, y-1));
                if(x-1>=0)
                    if(board_matrix[(x-1)+(y-1)*8]>0)
                        player2_possible[i].push_back(make_pair(x-1, y-1));
            }
        }
    }
}


void Engine::EvaluateBishops1()
{
    for(int i=9;i<=10;i++)
    {
        player1_possible[i].clear();
        int x = player1_current[i].first;
        int y = player1_current[i].second;
        for(int j=-1;j<=1;j+=2)
            for(int k=-1;k<=1;k+=2)
                for(int inc1 = j, inc2 = k ; (x+inc1>=0 && x+inc1<8) && (y+inc2>=0 && y+inc2<8);inc1+=j, inc2+=k)
                {
                    if(board_matrix[(x+inc1) + (y+inc2)*8]>0)
                        break;
                    else if (board_matrix[(x+inc1) + (y+inc2)*8]<0)
                    {
                        player1_possible[i].push_back(make_pair(x+inc1, y+inc2));
                        break;
                    }
                    player1_possible[i].push_back(make_pair(x+inc1, y+inc2));
                }
    }
}

void Engine::EvaluateBishops2()
{

    for(int i=9;i<=10;i++)
    {
        player2_possible[i].clear();
        int x = player2_current[i].first;
        int y = player2_current[i].second;
        for(int j=-1;j<=1;j+=2)
            for(int k=-1;k<=1;k+=2)
                for(int inc1 = j, inc2 = k ; (x+inc1>=0 && x+inc1<8) && (y+inc2>=0 && y+inc2<8);inc1+=j, inc2+=k)
                {
                    if(board_matrix[(x+inc1) + (y+inc2)*8]<0)
                        break;
                    else if (board_matrix[(x+inc1) + (y+inc2)*8]>0)
                    {
                        player2_possible[i].push_back(make_pair(x+inc1, y+inc2));
                        break;
                    }
                    player2_possible[i].push_back(make_pair(x+inc1, y+inc2));
                }
    }
}

void Engine::EvaluateKnights1()
{
    for(int i=11;i<=12;i++)
    {
        player1_possible[i].clear();
        int x = player1_current[i].first;
        int y = player1_current[i].second;

        if(x>=0 && y>=0)
        {
            for(int j=-2;j<=2;j++)
                for(int k=-2;k<=2;k++)
                {
                    if(j!=k && (abs(j)+abs(k)==3))
                    {
                        if(x+j>=0 && x+j<8 && y+k>=0 && y+k<8)
                            if(board_matrix[(x+j)+(y+k)*8]<=0)
                                player1_possible[i].push_back(make_pair(x+j, y+k));
                    }
                }
        }
    }
}

void Engine::EvaluateKnights2()
{
     for(int i=11;i<=12;i++)
    {
        player2_possible[i].clear();
        int x = player2_current[i].first;
        int y = player2_current[i].second;

        if(x>=0 && y>=0)
        {
            for(int j=-2;j<=2;j++)
                for(int k=-2;k<=2;k++)
                {
                    if(j!=k && (abs(j)+abs(k)==3))
                    {
                        if(x+j>=0 && x+j<8 && y+k>=0 && y+k<8)
                            if(board_matrix[(x+j)+(y+k)*8]>=0)
                                player2_possible[i].push_back(make_pair(x+j, y+k));
                    }
                }
        }
    }
}

void Engine::EvaluateRooks1()
{
    for(int i=13;i<=14;i++)
    {
        player1_possible[i].clear();
        int x = player1_current[i].first;
        int y = player1_current[i].second;
        for(int j=-1;j<=1;j++)
            for(int k=-1;k<=1;k++)
                for(int inc1 = j, inc2 = k ; (x+inc1>=0 && x+inc1<8) && (y+inc2>=0 && y+inc2<8);inc1+=j, inc2+=k)
                {
                    if(j==0 && k==0)
                        break;


                    //Eliminate cross movement
                    if(j!=0 && k!=0)
                        break;
                    if(board_matrix[(x+inc1) + (y+inc2)*8]>0)
                        break;
                    else if (board_matrix[(x+inc1) + (y+inc2)*8]<0)
                    {
                        player1_possible[i].push_back(make_pair(x+inc1, y+inc2));
                        break;
                    }
                    player1_possible[i].push_back(make_pair(x+inc1, y+inc2));
                }
    }
}

void Engine::EvaluateRooks2()
{
    for(int i=13;i<=14;i++)
    {
        player2_possible[i].clear();
        int x = player2_current[i].first;
        int y = player2_current[i].second;
        for(int j=-1;j<=1;j++)
            for(int k=-1;k<=1;k++)
                for(int inc1 = j, inc2 = k ; (x+inc1>=0 && x+inc1<8) && (y+inc2>=0 && y+inc2<8);inc1+=j, inc2+=k)
                {
                    if(j==0 && k==0)
                        break;


                    //Eliminate cross movement
                    if(j!=0 && k!=0)
                        break;
                    if(board_matrix[(x+inc1) + (y+inc2)*8]<0)
                        break;
                    else if (board_matrix[(x+inc1) + (y+inc2)*8]>0)
                    {
                        player2_possible[i].push_back(make_pair(x+inc1, y+inc2));
                        break;
                    }
                    player2_possible[i].push_back(make_pair(x+inc1, y+inc2));
                }
    }
}

void Engine::EvaluateQueen1()
{
    player1_possible[15].clear();
    int x = player1_current[15].first;
    int y = player1_current[15].second;
    for(int j=-1;j<=1;j++)
        for(int k=-1;k<=1;k++)
            for(int inc1 = j, inc2 = k ; (x+inc1>=0 && x+inc1<8) && (y+inc2>=0 && y+inc2<8);inc1+=j, inc2+=k)
            {
                if(j==0 && k==0)
                    break;
                if(board_matrix[(x+inc1) + (y+inc2)*8]>0)
                    break;
                else if (board_matrix[(x+inc1) + (y+inc2)*8]<0)
                {
                    player1_possible[15].push_back(make_pair(x+inc1, y+inc2));
                    break;
                }
                player1_possible[15].push_back(make_pair(x+inc1, y+inc2));
            }
}

void Engine::EvaluateQueen2()
{
     player2_possible[15].clear();
    int x = player2_current[15].first;
    int y = player2_current[15].second;
    for(int j=-1;j<=1;j++)
        for(int k=-1;k<=1;k++)
            for(int inc1 = j, inc2 = k ; (x+inc1>=0 && x+inc1<8) && (y+inc2>=0 && y+inc2<8);inc1+=j, inc2+=k)
            {
                if(j==0 && k==0)
                    break;
                if(board_matrix[(x+inc1) + (y+inc2)*8]<0)
                    break;
                else if (board_matrix[(x+inc1) + (y+inc2)*8]>0)
                {
                    player2_possible[15].push_back(make_pair(x+inc1, y+inc2));
                    break;
                }
                player2_possible[15].push_back(make_pair(x+inc1, y+inc2));
            }
}


void Engine::EvaluateKing1()
{
     player1_possible[16].clear();
    int x = player1_current[16].first;
    int y = player1_current[16].second;
    for(int j=-1;j<=1;j++)
        for(int k=-1;k<=1;k++)
        {
            if(j==0 && k==0)
                continue;
            if(((x+j)>=0 && (x+j)<8)&&((y+k)>=0 && (y+k)<8))
            {
                if(board_matrix[(x+j) + (y+k)*8]>0)
                    continue;

                player1_possible[16].push_back(make_pair(x+j, y+k));
            }

        }

}

void Engine::EvaluateKing2()
{
    player2_possible[16].clear();
    int x = player2_current[16].first;
    int y = player2_current[16].second;
    for(int j=-1;j<=1;j++)
        for(int k=-1;k<=1;k++)
        {
            if(j==0 && k==0)
                continue;
            if(((x+j)>=0 && (x+j)<8)&&((y+k)>=0 && (y+k)<8))
            {
                if(board_matrix[(x+j) + (y+k)*8]<0)
                    continue;

                player2_possible[16].push_back(make_pair(x+j, y+k));
            }

        }
}

bool Engine::CheckIfCheck1()
{
    EvaluateAllPlayer2Raw();

    int king_pos_x = player1_current[16].first;
    int king_pos_y = player1_current[16].second;

    bool check = false;
    for(int i=1;i<=16;i++)
    {
        for(vector<pair<int,int> >::iterator it = player2_possible[i].begin();it!=player2_possible[i].end();++it)
        {
            if(it->first == king_pos_x && it->second == king_pos_y)
            {
                check = true;
                break;
            }
        }
        if(check)
            break;
    }

    return check;
}

bool Engine::CheckIfCheck2()
{
    EvaluateAllPlayer1Raw();

    int king_pos_x = player2_current[16].first;
    int king_pos_y = player2_current[16].second;

    bool check = false;
    for(int i=1;i<=16;i++)
    {
        for(vector<pair<int,int> >::iterator it = player1_possible[i].begin();it!=player1_possible[i].end();++it)
        {
            if(it->first == king_pos_x && it->second == king_pos_y)
            {
                check = true;
                break;
            }
        }
        if(check)
            break;
    }

    return check;
}


#endif // _EVALUATION_H
