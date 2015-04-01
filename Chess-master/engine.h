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

//Initial positions of the pieces of player 1
const int initial_position1[][2] = {  {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1},
                                {2, 0}, {5, 0}, {1, 0}, {6, 0}, {0, 0}, {7, 0}, {3, 0}, {4, 0}};

//Initial positions of the pieces of player 2
const int initial_position2[][2] = {   {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {7, 6},
                                {2, 7}, {5, 7}, {1, 7}, {6, 7}, {0, 7}, {7, 7}, {3, 7}, {4, 7}};


//Contains info as to which type of character the elements in the vectors store
//( As in the vectors which store possible moves and current positions )
int map_vector_to_board1[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6 };
int map_vector_to_board2[] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -3, -3, -4, -4, -5, -6};

struct moves
{
    int piece_id;
    int x0;
    int y0;
    int x1;
    int y1;

    moves(){};
    moves(int Piece_id,int X0, int Y0, int X1, int Y1)
    {
        piece_id = Piece_id;
        x0 = X0;
        y0 = Y0;
        x1 = X1;
        y1 = Y1;
    }
};

class Engine
{
    private:
    int* board_matrix;
    int* selection_matrix;

    //Needed for handling user interface.
    bool selected = false;
    pair<int,int> selected_block;

    //Stores the current positions of the pieces. Position is (-1, -1) if piece is removed.
    pair<int,int> *player1_current;
    pair<int,int> *player2_current;

    //Stores all possible moves at a given time
    vector<pair<int,int> > *player1_possible;
    vector<pair<int,int> > *player2_possible;

    //Stores moves made previously
    vector<moves> previous_moves;

    //Evaluates all possible moves. But also removes illegal moves that endanger the king and adds special moves like castling
    void EvaluateAllPlayer1();
    void EvaluateAllPlayer2();

    //Evaluates all possible moves. DOES NOT remove moves that endanger the king
    void EvaluateAllPlayer1Raw();
    void EvaluateAllPlayer2Raw();

    //Evaluates different types of pieces
    //Player 1 and Player 2 have separate evaluation functions
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

    //Basically evaluates the state of the game at a given point
    //Assumes that moves have already been evaluated
    int EvaluateGameState1();
    int EvaluateGameState2();

    //Addition for special moves
    void EvaluateCastle1();
    void EvaluateCastle2();
    void EvaluateEnPassant1();
    void EvaluateEnPassant2();

    bool HandleCastle1(int x0 ,int y0, int x1, int y1);
    bool HandleCastle2(int x0, int y0, int x1, int y1);
    bool HandleEnPassant1(int x0, int y0, int x1, int y1);
    bool HandleEnPassant2(int x0, int y0, int x1, int y1);

    bool CheckIfMoved(int index);

    bool CheckIfCheck1();
    bool CheckIfCheck2();
    void RemoveIllegalMoves1();
    void RemoveIllegalMoves2();
    bool updated = false;

    int player1 = true;
    void UpdateBoardMatrix();

    void MakeMove(int x0, int y0, int x1, int y1);

    //Functions which do not change player and evaluate moves for the next player. Return value is index of the piece removed, if any.
    int MakeMove1(int x0, int y0, int x1, int y1);
    int MakeMove2(int x0, int y0, int x1, int y1);

    int game_status = PLAY_PLAYER1;

    int maximize(int depth);
    int minimize(int depth);

    moves minimax_base(int depth);
    void backtrack_moves(int previous_size);

    public:
    Engine();
    int GetPiece(int x,int y);
    int GetColor(int x,int y);
    void ProcessInput(int x,int y);
    bool CheckIfUpdated();
    int GetGameStatus();

    ~Engine();
};

//Initializes the variables required for the game
Engine::Engine()
{
    board_matrix = new int[64];
    for(int i=0;i<64;i++)
        board_matrix[i] = 0;

    selection_matrix = new int[64];
    for(int i=0;i<64;i++)
        selection_matrix[i] = 0;

    previous_moves.clear();

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

//Only used during initializatoin
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

//Returns which piece is present on the board at coordinates (x,y)
int Engine::GetPiece(int x,int y)
{
    int p = board_matrix[x + y*8];
    if(p>=0)
        return map_vector_to_board1[p];
    else if(p<0)
        return map_vector_to_board2[-p];
    return 0;
}

//Added for user interface.
//Returns 1 if the block has been selected
//Returns 2 if the blocks correspond to possible moves that can be made from the selected block
//Returns 0 otherwise
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

//Destructor.
Engine::~Engine()
{
    delete[] board_matrix;
}

//Makes a move ASSUMING it is valid and has already been validated by the Evaluation functions
void Engine::MakeMove(int x0, int y0, int x1, int y1)
{
    if(player1)
    {
        MakeMove1(x0, y0, x1, y1);
        player1 = false;
        EvaluateAllPlayer2();
    }
    else
    {
        MakeMove2(x0, y0, x1, y1);
        player1 = true;
        EvaluateAllPlayer1();
    }
}

int Engine::MakeMove1(int x0, int y0, int x1, int y1)
{
    int return_value = 0;
    bool move_handled = false;

    move_handled = HandleCastle1(x0, y0, x1, y1);
    if(move_handled == false)
        move_handled = HandleEnPassant1(x0, y0, x1, y1);

    if(move_handled == false)
    {
        int index = board_matrix[x0 + y0*8];
        int index2 = -board_matrix[x1 + y1*8];
        if(index2>0)
        {
            player2_current[index2] = make_pair(-1, -1);
            player2_possible[index2].clear();
            previous_moves.push_back(moves(-index2, x1, y1, -1, -1));
            return_value = -index2;
        }
        board_matrix[x0 + y0*8] = 0;
        board_matrix[x1 + y1*8] = index;

        player1_current[index] = make_pair(x1, y1);
        previous_moves.push_back(moves(index, x0, y0, x1, y1));
    }

    return return_value;
}

int Engine::MakeMove2(int x0, int y0, int x1, int y1)
{
    int return_value = 0;
    bool move_handled = false;

    move_handled = HandleCastle2(x0, y0, x1, y1);
    if(move_handled == false)
        move_handled = HandleEnPassant2(x0, y0, x1, y1);
    if(move_handled==false)
    {
        int index = -board_matrix[x0 + y0*8];
        int index2 = board_matrix[x1 + y1*8];
        if(index2>0)
        {
            player1_current[index2] = make_pair(-1, -1);
            player1_possible[index2].clear();
            previous_moves.push_back(moves(index2, x1, y1, -1, -1));
            return_value = index2;
        }

        board_matrix[x0 + y0*8] = 0;
        board_matrix[x1 + y1*8] = -index;

        player2_current[index] = make_pair(x1, y1);
        previous_moves.push_back(moves(-index, x0, y0, x1, y1));
    }

    return return_value;
}
//Assumes castling is possible
bool Engine::HandleCastle1(int x0, int y0, int x1, int y1)
{
    if(x0 == 4 && y0 == 0 && board_matrix[x0 + y0*8]==16)
    {

        if(x1 == 2 && y1 == 0)
        {
            //Move rook
            int index_rook = board_matrix[0 + 0*8];
            board_matrix[0 + 0*8] = 0;
            board_matrix[3 + 0*8] = index_rook;

            player1_current[index_rook] = make_pair(3, 0);
            previous_moves.push_back(moves(index_rook, 0, 0, 3, 0));

            //Move king
            board_matrix[4 + 0*8] = 0;
            board_matrix[2 + 0*8] = 16;
            player1_current[16] = make_pair(2,0);
            previous_moves.push_back(moves(16, 4, 0, 2, 0));

            return true;
        }
        else if(x1==6 && y1==0)
        {
            //Move rook
            int index_rook = board_matrix[7 + 0*8];
            board_matrix[7 + 0*8] = 0;
            board_matrix[5 + 0*8] = index_rook;

            player1_current[index_rook] = make_pair(5, 0);
            previous_moves.push_back(moves(index_rook, 7, 0, 5, 0));

            //Move king
            board_matrix[4 + 0*8] = 0;
            board_matrix[6 + 0*8] = 16;
            player1_current[16] = make_pair(6, 0);
            previous_moves.push_back(moves(16, 4, 0, 6, 0));
            return true;
        }
    }

    //Move not handled
    return false;
}

bool Engine::HandleCastle2(int x0, int y0, int x1, int y1)
{
    if(x0 == 4 && y0 == 7 && board_matrix[x0 + y0*8]==-16)
    {

        if(x1 == 2 && y1 == 7)
        {
            //Move rook
            int index_rook = -board_matrix[0 + 7*8];
            board_matrix[0 + 7*8] = 0;
            board_matrix[3 + 7*8] = -index_rook;

            player2_current[index_rook] = make_pair(3, 7);
            previous_moves.push_back(moves(-index_rook, 0, 7, 3, 7));

            //Move king
            board_matrix[4 + 7*8] = 0;
            board_matrix[2 + 7*8] = -16;
            player2_current[16] = make_pair(2,7);
            previous_moves.push_back(moves(-16, 4, 7, 2, 7));
            return true;
        }
        else if(x1==6 && y1==7)
        {
            //Move rook
            int index_rook = -board_matrix[7 + 7*8];
            board_matrix[7 + 7*8] = 0;
            board_matrix[5 + 7*8] = -index_rook;

            player2_current[index_rook] = make_pair(5, 7);
            previous_moves.push_back(moves(-index_rook, 7, 7, 5, 7));

            //Move king
            board_matrix[4 + 7*8] = 0;
            board_matrix[6 + 7*8] = -16;
            player2_current[16] = make_pair(6, 7);
            previous_moves.push_back(moves(-16, 4, 7, 6, 7));
            return true;
        }
    }

    //Move not handled
    return false;
}

bool Engine::HandleEnPassant1(int x0, int y0, int x1, int y1)
{
    if(y0 == 4 && y1== 5 && map_vector_to_board1[abs(board_matrix[x0 + y0*8])]==1)
    {
        //Differentiate between pawn capture and en passant
        if(map_vector_to_board2[abs(board_matrix[x1 + y0*8])] == -1 && map_vector_to_board2[abs(board_matrix[x1 + y1*8])]==0)
        {
            //Yes. Handle en passant
            int index_pawn1 = board_matrix[x0 + y0*8];
            int index_pawn2 = -board_matrix[x1 + y0*8];

            //Remove opponents pawn
            player2_current[index_pawn2] = make_pair(-1, -1);
            board_matrix[x1 + y0*8]=0;
            previous_moves.push_back(moves(-index_pawn2, x1, y0, -1, -1));

            //Move current pawn
            player1_current[index_pawn1] = make_pair(x1, y1);
            board_matrix[x0 + y0*8] = 0;
            board_matrix[x1 + y1*8] = index_pawn1;
            previous_moves.push_back(moves(index_pawn1, x0, y0, x1, y1));

            return true;
        }
    }

    return false;
}

bool Engine::HandleEnPassant2(int x0, int y0, int x1, int y1)
{
    if(y0 == 3 && y1== 2 && map_vector_to_board2[abs(board_matrix[x0 + y0*8])]==-1)
    {
        //Differentiate between pawn capture and en passant
        if(map_vector_to_board1[abs(board_matrix[x1 + y0*8])] == 1 && map_vector_to_board1[abs(board_matrix[x1 + y1*8])]==0)
        {
            //Yes. Handle en passant
            int index_pawn1 = -board_matrix[x0 + y0*8];
            int index_pawn2 = board_matrix[x1 + y0*8];

            //Remove opponents pawn
            player1_current[index_pawn2] = make_pair(-1, -1);
            board_matrix[x1 + y0*8]=0;
            previous_moves.push_back(moves(index_pawn2, x1, y0, -1, -1));

            //Move current pawn
            player2_current[-index_pawn1] = make_pair(x1, y1);
            board_matrix[x0 + y0*8] = 0;
            board_matrix[x1 + y1*8] = -index_pawn1;
            previous_moves.push_back(moves(-index_pawn1, x0, y0, x1, y1));

            return true;
        }
    }

    return false;
}
//Returns the current status of the game
int Engine::GetGameStatus()
{
    return game_status;
}
#endif _ENGINE_H
