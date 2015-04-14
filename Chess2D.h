#ifndef _CHESS2D_H
#define _CHESS2D_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/glfw3.h>
#include <cstdio>
#include <bits/stdc++.h>
#include <windows.h>

#include "image_loader.h"
#include "engine.h"
#include "shader_source.h"
#include "gl.h"

using namespace std;

#define EVENT_PROCESS_WAIT_TIME 0.03

#define AI_EASY 1
#define AI_MEDIUM 2
#define AI_HARD 3

#define AI_OPENING 10
#define AI_MIDDLE_GAME 11
#define AI_END_GAME 12

#define START_WINDOW 1
#define START_WINDOW_ONE_PLAYER_EASY 1
#define START_WINDOW_ONE_PLAYER_MEDIUM 2
#define START_WINDOW_ONE_PLAYER_HARD 3
#define START_WINDOW_TWO_PLAYER 4
#define START_WINDOW_EXIT_GAME 5

#define PLAY_WINDOW 2

#define RESUME_WINDOW 3
#define RESUME_WINDOW_RESUME 1
#define RESUME_WINDOW_UNDO 2
#define RESUME_WINDOW_BACK 3
#define RESUME_WINDOW_EXIT 4

#define CHECKMATE1_WINDOW 4
#define CHECKMATE1_WINDOW_BACK 1
#define CHECKMATE1_WINDOW_EXIT 2

#define CHECKMATE2_WINDOW 5
#define CHECKMATE2_WINDOW_BACK 1
#define CHECKMATE2_WINDOW_EXIT 2

#define STALEMATE_WINDOW 6
#define STALEMATE_WINDOW_BACK 1
#define STALEMATE_WINDOW_EXIT 2

#define PAWN1_WINDOW 7
#define PAWN1_KNIGHT 1
#define PAWN1_ROOK 2
#define PAWN1_QUEEN 3
#define PAWN1_BISHOP 4

#define PAWN2_WINDOW 8
#define PAWN2_KNIGHT 1
#define PAWN2_ROOK 2
#define PAWN2_QUEEN 3
#define PAWN2_BISHOP 4

struct RGBA
{
    float r;
    float g;
    float b;
    float a;

    RGBA() {};

    RGBA(float R, float G, float B) { r = R; b = B; g = G; }
};

float textures[][8] = {  {0, 0,          0, 0,           0, 0,       0,0        },
                        {5.0/6.0,0.5,   1.0, 0.5,       1.0, 0,     5.0/6.0,0   },
                        {3.0/6.0,0.5,   4.0/6.0,0.5,    4.0/6.0,0,  3.0/6.0,0   },
                        {4.0/6.0,0.5,   5.0/6.0,0.5,    5.0/6.0,0,  4.0/6.0,0   },
                        {2.0/6.0,0.5,   3.0/6.0,0.5,    3.0/6.0,0,  2.0/6.0,0   },
                        {1.0/6.0,0.5,   2.0/6.0,0.5,    2.0/6.0,0,  1.0/6.0,0   },
                        {0.0/6.0,0.5,   1.0/6.0,0.5,    1.0/6.0,0,  0,0         },
                        {5.0/6.0,1.0,   1.0,1.0,        1.0,0.5,    5.0/6.0,0.5 },
                        {3.0/6.0,1.0,   4.0/6.0,1.0,    4.0/6.0,0.5,3.0/6.0,0.5 },
                        {4.0/6.0,1.0,   5.0/6.0,1.0,    5.0/6.0,0.5,4.0/6.0,0.5 },
                        {2.0/6.0,1.0,   3.0/6.0,1.0,    3.0/6.0,0.5,2.0/6.0,0.5 },
                        {1.0/6.0,1.0,   2.0/6.0,1.0,    2.0/6.0,0.5,1.0/6.0,0.5 },
                        {0.0/6.0,1.0,   1.0/6.0,1.0,    1.0/6.0,0.5,0,0.5       } };

RGBA white(0.8,0.8,0.8);
RGBA black(0.4, 0.4, 0.4);
RGBA selected_color(0.1, 0.8, 0.1);
RGBA highlighted_color(0.3, 1.0, 0.3);

class Chess2D
{
private:
    static float board_left;
    static float board_top;
    static float board_width;
    static float board_height;

    static int window_width;
    static int window_height;
    GLuint vao;
    GLuint vbo;
    GLuint eao;
    Shader *board_shader;
    GLFWwindow *window;


    //static GLfloat *vertices;
    vector<float> vertices;
    unsigned int num_vertices;

    vector<int> elements;
    unsigned int num_elements;

    static Engine *engine;
    void LoadBoardColor();
    void LoadPieceTextures();
    void DrawBoard();
    static void ProcessMouseInput(GLFWwindow*,int,int,int);
    static void ProcessKeyInput(GLFWwindow*,int,int,int,int);
    static bool selected;
    static int prev_x;
    static int prev_y;

    static moves pawn_promo_move;

    static int current_player;

    static bool single_player_mode;
    static int display_status;

    static vector<ImageMenu*> menu;
    int MainLoop(GLFWwindow*);
public:
    Chess2D(int width = 800 ,int height = 600,int boardwidth = 600,int boardheight = 600);

    int StartGame();

    ~Chess2D();
};

int Chess2D::window_width = 0;
int Chess2D::window_height = 0;
float Chess2D::board_width = 0;
float Chess2D::board_height = 0;
float Chess2D::board_left = 0;
float Chess2D::board_top = 0;

Engine* Chess2D::engine = NULL;

bool Chess2D::selected = false;
int Chess2D::prev_x = 0;
int Chess2D::prev_y = 0;
int Chess2D::current_player = 1;

bool Chess2D::single_player_mode = false;
int Chess2D::display_status = 0;
vector<ImageMenu*> Chess2D::menu;
moves Chess2D::pawn_promo_move = moves(-1,-1,-1,-1);

//Load vertex shader, Fragment shader, set glfw window hints, initialize data
Chess2D::Chess2D(int width, int height, int boardwidth, int boardheight)
{
    window_width = width;
    window_height = height;
    board_width = (float)boardwidth*2/window_width;
    board_height = (float)boardheight*2/window_height;
    board_left = -board_width/2;
    board_top = board_height/2;

    //Initialize and load data. Each square has 4 vertices
    //Position: 2 floats
    //Colour: 3 floats
    //Texture: 2 floats

    vertices.resize(4*64*7);
    for(int i=0;i<4*64*7;i++)
        vertices[i] = 0.0f;

    int index = 0;
    for(int j=7;j>=0;j--)
        for(int i=0;i<8;i++)
        {

            float x = i, y=j;
            vertices[index] = board_left + board_width*x/8;
            vertices[index+1 ] = board_top - board_height*y/8;

            x++; index+=7;
            vertices[index] = board_left + board_width*x/8;
            vertices[index+1 ] = board_top - board_height*y/8;

            y++;index+=7;
            vertices[index] = board_left + board_width*x/8;
            vertices[index+1 ] = board_top - board_height*y/8;

            x--; index+=7;
            vertices[index] = board_left + board_width*x/8;
            vertices[index+1 ] = board_top - board_height*y/8;

            index+=7;
        }
    num_vertices = 4*8*8*7;
    elements.resize(6*64);


    index = 0;
    GLuint index2 = 0;
    for(int i=0;i<64;i++)
    {
        elements[index2] = index;
        elements[index2+1] = index+1;
        elements[index2+2] = index+2;
        elements[index2+3] = index;
        elements[index2+4] = index+2;
        elements[index2+5] = index+3;
        index+=4;
        index2+=6;
    }
    num_elements = 8*8*6;

    //Load engine for chess
    engine = new Engine(true);
    LoadBoardColor();
    LoadPieceTextures();


     if(!glfwInit())
    {
        fputs("Unable to load GLFW\n", stderr);
        return;
    }
    //Set appropriate window hints to
    //load OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //Create window
    //And set appropriate window settings
    window = glfwCreateWindow(window_width, window_height, "Chess 2D", NULL, NULL);
    if(!window)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
        glfwWindowHint(GLFW_VISIBLE, false);

        window = glfwCreateWindow(window_width, window_height, "Chess 2D", NULL, NULL);

        if(!window)
        {
            fputs("Unable to create window\n", stderr);
            return;
        }
    }

    //Set callback functions for
    //Mouse and keyboard events

    glfwSetMouseButtonCallback(window, ProcessMouseInput);
    glfwSetKeyCallback(window, ProcessKeyInput);
    glfwMakeContextCurrent(window);

    //Initialize glew to
    //Link to OpenGL functions
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK)
    {
        fputs("Unable to load GLEW\n", stderr);
        return;
    }

     //Load Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Load Vertex Buffer Objects
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    //Load Vertex Element Objects
    glGenBuffers(1, &eao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_elements*sizeof(GLuint), &elements[0], GL_STATIC_DRAW);

    //Load Shaders
    board_shader = new Shader(vertex_shader_source, fragment_shader_source);
    board_shader->Load();

    //Load Textures
    GLuint tex;
    glGenTextures(1, &tex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    //Load paramter for the texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Load the texture file
    int tex_width, tex_height;
    unsigned char* pixels = loadBMP("images/chess.bmp", &tex_width, &tex_height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    unload_BMP(pixels);
    //Set position attribute
    GLint posAttrib = glGetAttribLocation(board_shader->id, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, (7*sizeof(GLfloat)), 0); //Now stores in the VAO vao
    glEnableVertexAttribArray(posAttrib);

    //Set color attributes for the board
    GLint colAttrib = glGetAttribLocation(board_shader->id, "color");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colAttrib);
    glfwSwapInterval(1);

    //Set texture attributes for the board
    GLuint texAttrib = glGetAttribLocation(board_shader->id, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(texAttrib);

    //Enable alpha ( transparency )
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Deallocate memory used up by the image file
    unload_BMP(pixels);

    //Load start menu
    {
        Rect<int> r{0, 0, 800, 600};
        Rect<int> op1{160, 190, 530, 47};
        Rect<int> op2{87, 264, 633, 49};
        Rect<int> op3{136, 347, 556, 50};
        Rect<int> op4{236, 432, 326, 52};
        Rect<int> op5{330, 520, 109, 53};

        vector<Rect<int> > options;
        options.push_back(op1);
        options.push_back(op2);
        options.push_back(op3);
        options.push_back(op4);
        options.push_back(op5);
        menu.push_back(new ImageMenu("images/start_menu.bmp", 800, 600, r, options));
    }

    //Load resume menu
    {
        Rect<int> r{0, 0, 800, 600};
        Rect<int> op1{166, 84, 524, 61};
        Rect<int> op2{300, 180, 198, 62};
        Rect<int> op3{57, 280, 708, 65};
        Rect<int> op4{330, 374, 146, 68};

        vector<Rect<int> > options;
        options.push_back(op1);
        options.push_back(op2);
        options.push_back(op3);
        options.push_back(op4);
        menu.push_back(new ImageMenu("images/resume_menu.bmp", 800, 600, r, options));
    }

    //Load checkmate menu where player 1 wins
    {
        Rect<int> r{0, 0, 800, 600};
        Rect<int> op1{101, 296, 632, 55};
        Rect<int> op2{328, 389, 124, 57};

        vector<Rect<int> > options;
        options.push_back(op1);
        options.push_back(op2);
        menu.push_back(new ImageMenu("images/checkmate1_menu.bmp", 800, 600, r, options));
    }

    //Load checkmate menu where player 2 wins
    {
        Rect<int> r{0, 0, 800, 600};
        Rect<int> op1{101, 296, 632, 55};
        Rect<int> op2{328, 389, 124, 57};

        vector<Rect<int> > options;
        options.push_back(op1);
        options.push_back(op2);
        menu.push_back(new ImageMenu("images/checkmate2_menu.bmp", 800, 600, r, options));
    }

    //Load stalemate menu
    {
        Rect<int> r{0, 0, 800, 600};
        Rect<int> op1{110, 296, 632, 55};
        Rect<int> op2{347, 389, 124, 57};

        vector<Rect<int> > options;
        options.push_back(op1);
        options.push_back(op2);
        menu.push_back(new ImageMenu("images/stalemate_menu.bmp", 800, 600, r, options));

    }

    //Load pawn promotion 1 menu
    {
        Rect<int> r{0, 0, 800, 600};
        Rect<int> op1{21, 271, 208, 195};
        Rect<int> op2{326,269, 192, 203};
        Rect<int> op3{581, 275, 188,185};
        vector<Rect<int> > options;
        options.push_back(op1);
        options.push_back(op2);
        options.push_back(op3);
        menu.push_back(new ImageMenu("images/pawn1_menu.bmp", 800, 600, r, options));
    }

    //Load pawn promotion 2 menu
    {
        Rect<int> r{0, 0, 800, 600};
        Rect<int> op1{21, 271, 208, 195};
        Rect<int> op2{326,269, 192, 203};
        Rect<int> op3{581, 275, 188,185};
        vector<Rect<int> > options;
        options.push_back(op1);
        options.push_back(op2);
        options.push_back(op3);
        menu.push_back(new ImageMenu("images/pawn2_menu.bmp", 800, 600, r, options));
    }
}

int Chess2D::StartGame()
{
    display_status = START_WINDOW;

    //Show window and start game
    glfwShowWindow(window);
    current_player = engine->GetCurrentPlayer();

    return MainLoop(window);
}

int Chess2D::MainLoop(GLFWwindow *window)
{
    //prev_event_time with glfwGetTime()
    //is used to create delay between
    //consecutive page refreshes

    double prev_event_time = glfwGetTime();

    //Text object which writes text on window
    Text text;
    while(!glfwWindowShouldClose(window))
    {
        current_player = engine->GetCurrentPlayer();

        //Clear color buffer bit i.e. previous drawings
        glClear(GL_COLOR_BUFFER_BIT);

        //Draw the correct window
        switch(display_status)
        {
        case PLAY_WINDOW:
        {

            DrawBoard();

            int game_status = engine->GetGameStatus();

            //Get status of the game
            //Game is stopped if there's either a checkmate or stalemate
            switch(game_status)
            {
            case GAME_CHECK:
                text.Draw(-0.2,0.5, 0.25, "Check!");
                break;
            case GAME_CHECKMATE:
                if(engine->GetCurrentPlayer()==1)
                {
                    display_status = CHECKMATE2_WINDOW;
                }
                else
                {
                    display_status = CHECKMATE1_WINDOW;

                }
                break;
            case GAME_STALEMATE:

                display_status = STALEMATE_WINDOW;
                break;
            }
        }
        break;

        //Draw the appropriate menu
        case START_WINDOW:
            menu[0]->DrawMenu();
            break;
        case RESUME_WINDOW:
            menu[1]->DrawMenu();
            break;
        case CHECKMATE1_WINDOW:
            menu[2]->DrawMenu();
            break;
        case CHECKMATE2_WINDOW:
            menu[3]->DrawMenu();
            break;
        case STALEMATE_WINDOW:
            menu[4]->DrawMenu();
            break;
        case PAWN1_WINDOW:
            menu[5]->DrawMenu();
            break;
        case PAWN2_WINDOW:
            menu[6]->DrawMenu();
            break;
        }

        //Swap buffers to display final window
        glfwSwapBuffers(window);


        //Create delay between consecutive refreshes
        while(glfwGetTime()-prev_event_time<EVENT_PROCESS_WAIT_TIME)
            Sleep(1);
        prev_event_time = glfwGetTime();

        glfwPollEvents();
    }

    return 0;
}
void Chess2D::DrawBoard()
{
    //Load texture for board
    //Basically the different pieces on the board
    LoadPieceTextures();

    //Load color for the board
    //This includes colors forselected pieces
    LoadBoardColor();

    //Load data to OpenGL Buffers
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * sizeof(GLfloat), &vertices[0]);

    //Finally draw the board
    glDrawElements(GL_TRIANGLES, 8*8*6, GL_UNSIGNED_INT, 0);
}

Chess2D::~Chess2D()
{
    //Call destructor for shader
    delete board_shader;

    //Delete buffers allocated for board
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &eao);
    glDeleteVertexArrays(1, &vao);


}

void Chess2D::LoadBoardColor()
{
    //default prev_ai_move is -2,-2,-2,-2.
    //Anything that is not legal
    static moves prev_ai_move = moves(-2,-2,-2,-2);
    moves ai_move;
    int index = 0;

    //Load selections for ai player's move
    //if single_player_mode is loaded
    if(single_player_mode)
        ai_move = engine->GetAIMove();

    //Load board colors
    for(int j=0;j<8;j++)
        for(int i = 0;i<8;i++)
        {

            RGBA board_color;

            if(selected)
            {

                bool possible_move = engine->IsValidMove(prev_x, prev_y, i, j);

                if(i==prev_x && j==prev_y)
                    board_color = selected_color;
                else if(possible_move)
                    board_color = highlighted_color;
                else
                    if((i+j)%2 == 0)
                        board_color = white;
                    else
                        board_color = black;
                //Discard AI move highlight
                prev_ai_move = ai_move;
            }
            else
            {
                if((i+j)%2 == 0)
                    board_color = white;
                else
                    board_color = black;

                //Also highlight moves made by computer in single player mode

                if(ai_move.x0 != prev_ai_move.x0 || ai_move.y0!= prev_ai_move.y0 || ai_move.x1 != prev_ai_move.x1 || ai_move.y1 !=prev_ai_move.y1)
                    if(single_player_mode)
                    {
                        if(i==ai_move.x0 && j==ai_move.y0)
                            board_color = highlighted_color;
                        else if(i==ai_move.x1 && j==ai_move.y1)
                            board_color = selected_color;

                    }


            }

            for(int k=0;k<4;k++)
            {
                vertices[index+2] = board_color.r;
                vertices[index+3] = board_color.g;
                vertices[index+4] = board_color.b;
                index+=7;
            }

        }



}


void Chess2D::LoadPieceTextures()
{
    //Load textures for board
    //These textures represent
    //the different pieces

    int index = 0;
    for(int j=0;j<8;j++)
        for(int i=0;i<8;i++)
        {
            int piece = engine->GetPiece(i,j);

            //Temporary
            if(piece<0)
                piece = 6 - piece;

            for(int k=0;k<4;k++)
            {
                vertices[index+5]=textures[piece][2*k];
                vertices[index+6]=textures[piece][2*k+1];
                index+=7;
            }
        }

}

void Chess2D::ProcessMouseInput(GLFWwindow *window, int button,int action,int mods)
{
    //Not take inputs when computer is thinking
    if(action ==GLFW_PRESS && engine->GetGameStatus()!=GAME_THINKING)
    {

        double x,y;

        //Get cursor position
        glfwGetCursorPos(window, &x, &y);

        //Process the input
        //correctly
        //based on the window being displayed
        switch(display_status)
        {
            case PLAY_WINDOW:
                x/=window_width;
                y/=window_height;
                x = 2*x - 1;
                y = 1-2*y;

                int square_x, square_y;
                square_x = (x-board_left)*8/board_width;
                square_y = (board_top - y)*8/board_height;

                //Convert to coordinates used by engine
                square_y = 7 - square_y;
                if((x>=board_left && x<=board_left+board_width && y<=board_top && y>=board_top - board_height))
                {
                    if(!selected)
                    {
                        if((current_player==1 && engine->GetPiece(square_x,square_y)>0) || (current_player==2 && engine->GetPiece(square_x, square_y)<0))
                        {
                            selected = true;
                            prev_x = square_x;
                            prev_y = square_y;
                        }
                    }
                    else
                    {
                        if(prev_x == square_x && prev_y == square_y)
                            selected = false;
                        else if((current_player==1 && engine->GetPiece(square_x,square_y)>0) || (current_player==2 && engine->GetPiece(square_x, square_y)<0))
                        {
                            selected = true;
                            prev_x = square_x;
                            prev_y = square_y;
                        }
                        else if(engine->IsValidMove(prev_x, prev_y, square_x, square_y))
                        {
                            selected = false;
                            if(engine->IsPawnPromotion(prev_x, prev_y, square_x, square_y))
                            {

                                pawn_promo_move = moves(prev_x, prev_y, square_x, square_y);
                                if(current_player == 1)
                                    display_status = PAWN1_WINDOW;
                                else if(current_player == 2)
                                    display_status = PAWN2_WINDOW;

                                break;
                            }
                            engine->ProcessInput(prev_x, prev_y, square_x, square_y);
                        }
                    }
                }
            break;
            case START_WINDOW:
            {
                int result = menu[0]->ProcessInput(2*x/window_width-1.0f, 1.0f-2*y/window_height);
                if(result==START_WINDOW_ONE_PLAYER_EASY)
                {
                    single_player_mode = true;
                    if(engine)
                        delete engine;
                    engine = new Engine(true, AI_EASY);
                    display_status = PLAY_WINDOW;
                }
                else if(result==START_WINDOW_ONE_PLAYER_MEDIUM)
                {
                    single_player_mode = true;
                    if(engine)
                        delete engine;
                    engine = new Engine(true, AI_MEDIUM);
                    display_status = PLAY_WINDOW;
                }
                else if(result==START_WINDOW_ONE_PLAYER_HARD)
                {
                    single_player_mode = true;
                    if(engine)
                        delete engine;
                    engine = new Engine(true, AI_HARD);
                    display_status = PLAY_WINDOW;
                }
                else if(result == START_WINDOW_TWO_PLAYER)
                {
                    single_player_mode = false;
                    if(engine)
                        delete engine;
                    engine = new Engine(false);
                    display_status = PLAY_WINDOW;
                }
                else if(result == START_WINDOW_EXIT_GAME)
                {
                    glfwSetWindowShouldClose(window, true);
                }
                break;
            }
            case RESUME_WINDOW:
            {
                int result = menu[1]->ProcessInput(2*x/window_width-1.0f, 1.0f-2*y/window_height);
                if(result == RESUME_WINDOW_RESUME)
                {
                    display_status = PLAY_WINDOW;
                }
                else if(result == RESUME_WINDOW_UNDO)
                {
                    bool modified = engine->UndoGame();
                    if(modified)
                        selected=false;
                    display_status = PLAY_WINDOW;
                }
                else if(result == RESUME_WINDOW_BACK)
                {
                    display_status = START_WINDOW;
                    selected = false;
                }
                else if(result == RESUME_WINDOW_EXIT)
                {
                    glfwSetWindowShouldClose(window, true);
                }
                break;
            }
            case CHECKMATE1_WINDOW:
            {
                int result = menu[2]->ProcessInput(2*x/window_width-1.0f, 1.0f-2*y/window_height);
                if(result==CHECKMATE1_WINDOW_BACK)
                {
                    display_status = START_WINDOW;
                    selected = false;
                }
                else if(result==CHECKMATE1_WINDOW_EXIT)
                {
                    glfwSetWindowShouldClose(window, true);
                }
                break;
            }
            case CHECKMATE2_WINDOW:
            {
                int result = menu[3]->ProcessInput(2*x/window_width-1.0f, 1.0f-2*y/window_height);
                if(result==CHECKMATE2_WINDOW_BACK)
                {
                    display_status = START_WINDOW;
                    selected = false;
                }
                else if(result==CHECKMATE2_WINDOW_EXIT)
                {
                    glfwSetWindowShouldClose(window, true);
                }
                break;
            }
            case STALEMATE_WINDOW:
            {
                int result = menu[4]->ProcessInput(2*x/window_width-1.0f, 1.0f-2*y/window_height);
                if(result==STALEMATE_WINDOW_BACK)
                {
                    display_status = START_WINDOW;
                    selected = false;
                }
                else if(result==STALEMATE_WINDOW_EXIT)
                {
                    glfwSetWindowShouldClose(window, true);
                }
                break;
            }
            case PAWN1_WINDOW:
            {
                int pawn_promo_code = 0;
                int result = menu[5]->ProcessInput(2*x/window_width-1.0f, 1.0f-2*y/window_height);
                switch(result)
                {
                case PAWN1_QUEEN:
                    pawn_promo_code = 0;
                    break;
                case PAWN1_BISHOP:
                    pawn_promo_code = 1;
                    break;
                case PAWN1_KNIGHT:
                    pawn_promo_code = 2;
                    break;
                case PAWN1_ROOK:
                    pawn_promo_code = 3;
                    break;
                }
                engine->ProcessInput(pawn_promo_move.x0, pawn_promo_move.y0, pawn_promo_move.x1, pawn_promo_move.y1, pawn_promo_code);
                display_status = PLAY_WINDOW;
                break;
            }
            case PAWN2_WINDOW:
            {
                int pawn_promo_code = 0;
                int result = menu[6]->ProcessInput(2*x/window_width-1.0f, 1.0f-2*y/window_height);
                switch(result)
                {
                case PAWN2_QUEEN:
                    pawn_promo_code = 0;
                    break;
                case PAWN2_BISHOP:
                    pawn_promo_code = 1;
                    break;
                case PAWN2_KNIGHT:
                    pawn_promo_code = 2;
                    break;
                case PAWN2_ROOK:
                    pawn_promo_code = 3;
                    break;
                }
                engine->ProcessInput(pawn_promo_move.x0, pawn_promo_move.y0, pawn_promo_move.x1, pawn_promo_move.y1, pawn_promo_code);
                display_status = PLAY_WINDOW;
                break;
            }
        }
    }
}

void Chess2D::ProcessKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //Ctrl-z is keyboard shortcut for undo
    if(key==GLFW_KEY_Z && mods==GLFW_MOD_CONTROL && action==GLFW_PRESS)
    {
        bool modified = engine->UndoGame();
        if(modified)
            selected=false;
    }
    else if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if(display_status == PLAY_WINDOW)
        {
            display_status = RESUME_WINDOW;
        }
        else if(display_status == RESUME_WINDOW)
        {
            display_status = PLAY_WINDOW;
        }

    }
}
#endif // _CHESS2D_H
