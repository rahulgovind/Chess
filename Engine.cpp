#include "engine.h"
#include "engine_ai.h"

using namespace std;

//constructor for engine
//AI_mode determines where the engine
//is in single player mode
//or two player mode
Engine::Engine(bool AI_mode, int difficulty):Board()
{

    player1 = true;
    game_status = GAME_NORMAL;

    prev_ai_move.x0 = -1;
    prev_ai_move.y0 = -1;
    prev_ai_move.x1 = -1;
    prev_ai_move.y1 = -1;

    ai_mode = AI_mode;
    ai_difficulty = difficulty;
}

//Engine constructor used for
//reconstructing old game
Engine::Engine(string fname, bool AI_mode, int difficulty)
{
    try
    {
        int x0, y0, x1, y1, pawn_promo;
        Engine temp(false);
        vector<moves> redo_moves;
        redo_moves.clear();
        vector<int> pawn_promos;
        ifstream dbg(fname.c_str());
        while(true)
        {
            dbg>>x0>>y0>>x1>>y1>>pawn_promo;
            if(dbg.eof())
                break;
            if(x0>=0 && x0<8 && y0>=0 && y0<8 && x1>=0 && x1<8 && y1>=0 && y1<8 && pawn_promo>=0 && pawn_promo<=3)
            {
                redo_moves.push_back(moves(x0, y0,x1, y1));
                pawn_promos.push_back(pawn_promo);
            }
        }

        int length = redo_moves.size();
        int i=0;
        for(vector<moves>::iterator it = redo_moves.begin();i<length-length%2;++it, ++i)
        {
            temp.ProcessInput(it->x0, it->y0, it->x1, it->y1, pawn_promos[i]);
        }
        *this = temp;
        this->ai_mode = AI_mode;

        printf("%u\n", length);
        if(length%2==1)
        {
            moves last_move = redo_moves.back();
            this->ProcessInput(last_move.x0, last_move.y0, last_move.x1, last_move.y1, pawn_promos.back());
        }

        dbg.close();
    }
    catch(...)
    {
        cout<<"Error reading from "<<fname<<endl;
    }
}

//Returns status of the game
//Like check or checkmate or stalemate
int Engine::GetGameStatus()
{
    return game_status;
}

//Returns which player is current playing
int Engine::GetCurrentPlayer()
{
    if(player1)
        return 1;
    else return 2;
}

//Returns the id of the piece
//at position (x,y) on the board

int Engine::GetPiece(int x, int y)
{
    return board_matrix[x][y];
}


//Process input given to the Engine
void Engine::ProcessInput(int x0, int y0, int x1, int y1, int pawn_promo)
{
    if((player1 && board_matrix[x0][y0]>0) || (!player1 && board_matrix[x0][y0]<0))
        if(IsValidMove(x0, y0, x1, y1))
        {
            if(ENGINE_DEBUG)
            {
                try
                {
                    ofstream dbg;
                    if(previous_moves.size()==0)
                        dbg.open("debug.dat", ios::trunc);
                    else
                        dbg.open("debug.dat", ios::app);
                    dbg<<x0<<"\t"<<y0<<"\t"<<x1<<"\t"<<y1<<"\t"<<pawn_promo<<"\n";
                    dbg.close();
                }
                catch(...)
                {
                    cout<<"Error writing to file"<<endl;
                }
            }
            MakeMove(x0, y0, x1, y1, pawn_promo);
            player1 = !player1;

            if((player1 && IsCheck1()) || (!player1 && IsCheck2()))
                game_status = GAME_CHECK;
            else
                game_status = GAME_NORMAL;

            if((player1 && IsCheckmate1()) || (!player1 && IsCheckmate2()))
                game_status = GAME_CHECKMATE;
            else if((player1 && IsStalemate1()) || (!player1 && IsStalemate2()))
                game_status = GAME_STALEMATE;

            //Just for debugging
            if(player1==false && ai_mode && game_status!=GAME_CHECKMATE && game_status!=GAME_STALEMATE)
            {
                game_status = GAME_THINKING;
                MakeAIMove();
            }
        }
}

//Make a move
//from x0, y0
//to x1, y1
//Does not check if the move is valid
//Returns either the id of the captured piece
//or special values for special moves
//like castling and en passant
int Board::MakeMove(int x0, int y0, int x1, int y1, int pawn_promo)
{
    previous_moves.push_back(moves(x0,y0,x1,y1));
    int temp = board_matrix[x1][y1];
    board_matrix[x1][y1] = board_matrix[x0][y0];
    board_matrix[x0][y0] = 0;

    if(y1 == 7 && board_matrix[x1][y1] == 1)
    {

        if(pawn_promo == 0)
        {
            //Handle pawn promotion for queen
            board_matrix[x1][y1] = 5;
            temp -= 9;
        }
        else if(pawn_promo == 1)
        {
            //Pawn promotion for bishop
            board_matrix[x1][y1] = 2;
            temp -= 16;
        }
        else if(pawn_promo == 2)
        {
            //Pawn promotion for knight
            board_matrix[x1][y1] = 3;
            temp -= 23;
        }
        else if(pawn_promo == 3)
        {
            //Pawn promotion for queen
            board_matrix[x1][y1] = 4;
            temp -=30;
        }
    }
    else if(y1 == 0 && board_matrix[x1][y1] == -1)
    {
        if(pawn_promo == 0)
        {
            //Handle pawn promotion for queen
            board_matrix[x1][y1] = -5;
            temp += 9;
        }
        else if(pawn_promo == 1)
        {
            //Pawn promotion for bishop
            board_matrix[x1][y1] = -2;
            temp += 16;
        }
        else if(pawn_promo == 2)
        {
            //Pawn promotion for knight
            board_matrix[x1][y1] = -3;
            temp += 23;
        }
        else if(pawn_promo == 3)
        {
            //Pawn promotion for queen
            board_matrix[x1][y1] = -4;
            temp +=30;
        }
    }
    else if(board_matrix[x1][y1]==6 && y1==0 && y0==0 && abs(x1-x0)==2)
    {
        //Handle Castling for player 1
        if(x1==2)
        {
            //Far castle
            board_matrix[0][0] = 0;
            board_matrix[3][0] = 4;
            temp = 7;
        }
        else if(x1==6)
        {
            //Near castle
            board_matrix[7][0] = 0;
            board_matrix[5][0] = 4;
            temp = 7;
        }
    }
    else if(board_matrix[x1][y1]==-6 && y1==7 && y0==7 && abs(x1-x0)==2)
    {
        //Handle Castling for player 2
        if(x1==2)
        {
            //Far castle
            board_matrix[0][7] = 0;
            board_matrix[3][7] = -4;
            temp = -7;
        }
        else if(x1==6)
        {
            //Near castle
            board_matrix[7][7] = 0;
            board_matrix[5][7] = -4;
            temp = -7;
        }
    }
    else if(board_matrix[x1][y1]==1 && temp == 0 && abs(x1 - x0) == 1)
    {
        //En passant
        board_matrix[x1][4] = 0;
        temp = 8;
    }
    else if(board_matrix[x1][y1]==-1 && temp == 0 && abs(x1 - x0) == 1)
    {
        //En passant
        board_matrix[x1][3] = 0;
        temp = -8;
    }

    return temp;
}

//Undo move
//Requires caller to know information
//about the move to undo
void Board::UndoMove(int piece0, int piece1, int x0, int y0, int x1, int y1)
{
    previous_moves.pop_back(); //temporary
    if(piece1 >=9 || piece1 <= -9)
    {
        //Pawn promotion undo
        if(piece1>=9 && piece1<16)
            piece1-=9;
        else if(piece1>=16 && piece1<23)
            piece1-=16;
        else if(piece1>=23 && piece1<30)
            piece1-=23;
        else if(piece1> -16 && piece1<=-9)
            piece1+=9;
        else if(piece1> -23 && piece1<=-16)
            piece1+=16;
        else if(piece1> -30 && piece1<=-23)
            piece1+=23;
    }
    else if(piece1 == 7)
    {
        piece1 = 0;
        if(x1 == 2)
        {
            //Far castle
            board_matrix[0][0] = 4;
            board_matrix[3][0] = 0;
        }
        else if(x1 == 6)
        {
            //Far castle
            board_matrix[7][0] = 4;
            board_matrix[5][0] = 0;
        }
    }
    else if(piece1 == -7)
    {
        piece1 = 0;
        if(x1 == 2)
        {
            //Far castle
            board_matrix[0][7] = -4;
            board_matrix[3][7] = 0;
        }
        else if(x1 == 6)
        {
            //Far castle
            board_matrix[7][7] = -4;
            board_matrix[5][7] = 0;
        }
    }
    else if(piece1 == 8)
    {
        piece1 = 0;
        board_matrix[x1][4] = -1;
    }
    else if(piece1 == -8)
    {
        piece1 = 0;
        board_matrix[x1][3] = 1;
    }
    board_matrix[x0][y0] = piece0;
    board_matrix[x1][y1] = piece1;

}

//Caller gets the previous move
//made by the AI
moves Engine::GetAIMove()
{
    return prev_ai_move;
}

//Thread function called
//which starts EngineAI object
//and looks for best move

long unsigned int __stdcall Engine::AIThread(void *input)
{
    Engine *main_engine = (Engine*)input;

    EngineAI *test_engine = new EngineAI((*(Board*)main_engine));

    printf("Thinking. \n");

    moves ai;

    float init_time = (float)clock()/CLOCKS_PER_SEC;

    ai = test_engine->GetBestMove(main_engine->ai_difficulty);
    test_engine->PrintInfo();



    main_engine->prev_ai_move = moves(ai.x0, ai.y0, ai.x1, ai.y1);
    main_engine->ProcessInput(ai.x0, ai.y0, ai.x1, ai.y1);

    return 0;
}

//Creates thread to look for AI Move
//and expects the thread function to
//make the move too
void Engine::MakeAIMove()
{
    //AIThread(this);

    CreateThread(NULL, 0, AIThread, (void*)this, 0, 0);
}

//Undo the game
//This works by replaying the moves from the start
//upto the previous move
//(or the move before the previous move for AI)
bool Engine::UndoGame()
{
    bool modified = false;
    int prev_length = previous_moves.size();
    if(prev_length>=1 && game_status!=GAME_CHECKMATE && game_status!=GAME_STALEMATE)
    {
        Engine temp(false);
        if(ai_mode==false)
        {
            previous_moves.pop_back();
            for(vector<moves>::iterator it = previous_moves.begin();it!=previous_moves.end();++it)
            {
                temp.MakeMove(it->x0, it->y0, it->x1, it->y1);
            }

            for(int i=0;i<8;i++)
                for(int j=0;j<8;j++)
                    board_matrix[i][j] = temp.board_matrix[i][j];
            modified = true;
            player1 = !player1;
        }
        else if(ai_mode==true && game_status!=GAME_THINKING)
        {
            previous_moves.pop_back();
            previous_moves.pop_back();
            for(vector<moves>::iterator it = previous_moves.begin();it!=previous_moves.end();++it)
            {
                temp.MakeMove(it->x0, it->y0, it->x1, it->y1);
            }

            for(int i=0;i<8;i++)
                for(int j=0;j<8;j++)
                    board_matrix[i][j] = temp.board_matrix[i][j];

            prev_ai_move = moves(-1,-1,-1,-1);
            modified = true;
        }

    }

    return modified;
}

bool Engine::IsPawnPromotion(int x0, int y0, int x1, int y1)
{

    if(IsValidMove(x0,y0,x1,y1))
    {
        int prev_0 = board_matrix[x0][y0];
        if(prev_0 == 1 && y0 == 6 && y1 == 7)
            return true;
        else if(prev_0 == -1 && y0 == 1 && y1 == 0)
            return true;
    }

    return false;
}
