#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "engine.h"

void Engine::EvaluateAllPlayer1()
{
    EvaluateAllPlayer1Raw();

    bool check = CheckIfCheck1();

    EvaluateCastle1();
    EvaluateEnPassant1();
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

    EvaluateCastle2();
    EvaluateEnPassant2();
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

//Evaluates each possible move individually
void Engine::EvaluatePawns1()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board1[i]==1)
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

}

void Engine::EvaluatePawns2()
{
     for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board2[i] == -1)
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
}

//Evaluates the moves in each direction in a sequential order.
//That is, all moves in a particular direction are evaluated before evaluating the moves in another direction
void Engine::EvaluateBishops1()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board1[i] == 2)
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
}

void Engine::EvaluateBishops2()
{

    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board2[i] == -2)
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
}


//All blocks in a 5x5 block with the knight at the center is evaliuated
//The possible moves of the knight have the property that
//1) The displacement in the x-direction and the y-direction are never equal
//2) The sum of the displacement in the x-direction and the y-direction adds up to 3
void Engine::EvaluateKnights1()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board1[i] == 3)
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
}

void Engine::EvaluateKnights2()
{
     for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board2[i] == -3)
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
}

//Moves in a particular direction are evaluated before evaluating moves in another direction
void Engine::EvaluateRooks1()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board1[i]==4)
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
}

void Engine::EvaluateRooks2()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board2[i]==-4)
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
}

//Moves in a particular direction are evaluated before evaluating moves in another direction
void Engine::EvaluateQueen1()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board1[i]==5)
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
}

void Engine::EvaluateQueen2()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board2[i] == -5)
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
}

//All possible moves withing a 3x3 block with the king at the centre are evaluated indivually
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

//Checks for check anyway
void Engine::EvaluateCastle1()
{
    if(!CheckIfCheck1())
    {
        int king_pos_x = player1_current[16].first;
        int king_pos_y = player1_current[16].second;

        if(king_pos_x == 4 && king_pos_y==0)
        {
            //Check for castle to left side
            //Check if positions in between are empty
            if(board_matrix[3 + 0*8] == 0 && board_matrix[2 + 0*8]==0 && board_matrix[1 + 0*8] == 0)
            {
                //Check if piece at end is rook
                int index_rook = board_matrix[0 + 0*8];
                if(index_rook>0)
                    if(map_vector_to_board1[index_rook]==4)
                        if(CheckIfMoved(16) == false && CheckIfMoved(index_rook) == false)
                            player1_possible[16].push_back(make_pair(2,0));
            }

            //Same logic as above
            if(board_matrix[5+ 0*8]==0 && board_matrix[6 + 0*8]==0)
            {
                int index_rook = board_matrix[7 + 0*8];
                if(index_rook >0)
                    if(map_vector_to_board1[index_rook] == 4)
                        if(CheckIfMoved(16) == false && CheckIfMoved(index_rook) == false)
                            player1_possible[16].push_back(make_pair(6,0));
            }
        }
    }
}

void Engine::EvaluateCastle2()
{
    if(!CheckIfCheck2())
    {
        int king_pos_x = player2_current[16].first;
        int king_pos_y = player2_current[16].second;

        if(king_pos_x == 4 && king_pos_y==7)
        {
            //Check for castle to left side
            //Check if positions in between are empty
            if(board_matrix[3 + 7*8] == 0 && board_matrix[2 + 7*8]==0 && board_matrix[1 + 7*8] == 0)
            {
                //Check if piece at end is rook
                int index_rook = -board_matrix[0 + 7*8];
                if(index_rook>0)
                    if(map_vector_to_board2[index_rook]==-4)
                        if(CheckIfMoved(-16) == false && CheckIfMoved(-index_rook) == false)
                            player2_possible[16].push_back(make_pair(2,7));
            }

            //Same logic as above
            if(board_matrix[5+ 7*8]==0 && board_matrix[6 + 7*8]==0)
            {
                int index_rook = -board_matrix[7 + 7*8];
                if(index_rook >0)
                    if(map_vector_to_board2[index_rook] == -4)
                        if(CheckIfMoved(-16) == false && CheckIfMoved(-index_rook) == false)
                            player2_possible[16].push_back(make_pair(6,7));
            }
        }
    }
}

void Engine::EvaluateEnPassant1()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board1[i] == 1)
        {
            if(player1_current[i].second==4)
            {
                int pawn_x = player1_current[i].first;

                //Check to left
                if(pawn_x-1>=0)
                    if(map_vector_to_board2[abs(board_matrix[(pawn_x -1 ) + 4*8])] == -1)
                    {
                        // Check if previous move made by piece was a double move
                        //Length of vector should be greater than zero since
                        //Otherwise it's not possible for pawn to reach y=4
                        vector<moves>::iterator it = previous_moves.end();
                        --it;

                        if(map_vector_to_board2[abs(it->piece_id)] == -1 && it->x1 == (pawn_x -1) && it->y0 == 6 && it->y1 == 4)
                            player1_possible[i].push_back(make_pair(pawn_x-1, 5));
                    }
                //Check to right
                if(pawn_x+1<8)
                    if(map_vector_to_board2[abs(board_matrix[(pawn_x + 1) + 4*8])] == -1)
                    {
                        vector<moves>::iterator it = previous_moves.end();
                        --it;

                        if(map_vector_to_board2[abs(it->piece_id)] == -1 && it->x1 == (pawn_x +1) && it->y0 == 6 && it->y1 == 4)
                            player1_possible[i].push_back(make_pair(pawn_x+1, 5));
                    }

            }
        }
    }
}

void Engine::EvaluateEnPassant2()
{
    for(int i=1;i<=16;i++)
    {
        if(map_vector_to_board2[i] == -1)
        {
            if(player2_current[i].second==3)
            {
                int pawn_x = player2_current[i].first;

                //Check to left
                if(pawn_x-1>=0)
                    if(map_vector_to_board1[abs(board_matrix[(pawn_x -1 ) + 3*8])] == 1)
                    {
                        // Check if previous move made by piece was a double move
                        //Length of vector should be greater than zero since
                        //Otherwise it's not possible for pawn to reach y=4
                        vector<moves>::iterator it = previous_moves.end();
                        --it;

                        if(map_vector_to_board1[abs(it->piece_id)] == 1 && it->x1 == (pawn_x -1) && it->y0 == 1 && it->y1 == 3)
                            player2_possible[i].push_back(make_pair(pawn_x-1, 2));
                    }
                //Check to right
                if(pawn_x+1<8)
                    if(map_vector_to_board1[abs(board_matrix[(pawn_x + 1) + 3*8])] == 1)
                    {
                        vector<moves>::iterator it = previous_moves.end();
                        --it;

                        if(map_vector_to_board1[abs(it->piece_id)] == 1 && it->x1 == (pawn_x +1) && it->y0 == 1 && it->y1 == 3)
                            player2_possible[i].push_back(make_pair(pawn_x+1, 2));
                    }

            }
        }
    }
}
//Evaluates all possible moves for the opponent. And checks if any of them can reach the king
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

//Evaluates all possible moves for the opponent. And checks if any of them can reach the king
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

bool Engine::CheckIfMoved(int index)
{
    for(vector<moves>::iterator it = previous_moves.begin();it!=previous_moves.end();++it)
    {
        if(it->piece_id == index)
            return true;
    }

    return false;
}



#endif // _EVALUATION_H
