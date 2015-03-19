#ifndef _CHESS2D_H
#define _CHESS2D_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/glfw3.h>
#include <cstdio>
#include <bits/stdc++.h>

#include "image_loader.h"
#include "engine.h"
#include "shader_source.h"

using namespace std;

#define EVENT_PROCESS_WAIT_TIME 0.03

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

    GLFWwindow *window;
    char* vertex_source;
    char* fragment_source;

    static GLfloat *vertices;
    static GLuint num_vertices;

    GLuint *elements;
    GLuint num_elements;

    static RGBA white;
    static RGBA black;
    static RGBA selected_color;
    static RGBA highlighted_color;

    static Engine *engine;
    void LoadBoardColor();
    void LoadPieceTextures();
    void DrawBoard();
    static
    void ProcessMouseInput(GLFWwindow*,int,int,int);

    static bool selected;
    static int prev_x;
    static int prev_y;


    static bool prev_selected;
    static int prev_game_state;
    static int current_player;

    static bool single_player_mode;

public:
    Chess2D(bool single_player=true,int width = 800 ,int height = 600,int boardwidth = 600,int boardheight = 600);

    int StartGame();

    ~Chess2D();
};

int Chess2D::window_width = 0;
int Chess2D::window_height = 0;
float Chess2D::board_width = 0;
float Chess2D::board_height = 0;
float Chess2D::board_left = 0;
float Chess2D::board_top = 0;
GLfloat* Chess2D::vertices = NULL;
GLuint Chess2D::num_vertices = 0;

Engine* Chess2D::engine = NULL;

RGBA Chess2D::white(0.8,0.8,0.8);
RGBA Chess2D::black(0.4, 0.4, 0.4);
RGBA Chess2D::selected_color(0.1, 0.8, 0.1);
RGBA Chess2D::highlighted_color(0.3, 1.0, 0.3);

bool Chess2D::selected = false;
int Chess2D::prev_x = 0;
int Chess2D::prev_y = 0;
bool Chess2D::prev_selected = false;
int Chess2D::current_player = 1;
int Chess2D::prev_game_state = 0;

bool Chess2D::single_player_mode = false;
//Load vertex shader, Fragment shader, set glfw window hints, initialize data
Chess2D::Chess2D(bool single_player, int width, int height, int boardwidth, int boardheight)
{
    single_player_mode = single_player;
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

    vertices = new GLfloat[4*64*7];
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
    elements = new GLuint[6*64];


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
    engine = new Engine(single_player_mode);
    LoadBoardColor();
    LoadPieceTextures();

}

int Chess2D::StartGame()
{


    if(!glfwInit())
    {
        fputs("Unable to load GLFW\n", stderr);
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(window_width, window_height, "Chess 2D", NULL, NULL);
    if(!window)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

        window = glfwCreateWindow(window_width, window_height, "Chess 2D", NULL, NULL);

        if(!window)
        {
            fputs("Unable to create window\n", stderr);
            return -1;
        }
    }

    glfwSetMouseButtonCallback(window, ProcessMouseInput);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK)
    {
        fputs("Unable to load GLEW\n", stderr);
        return -1;
    }



    //Load Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Load Vertex Buffer Objects
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    printf("No. of vertices: %d\n", num_vertices/7);
    glBufferData(GL_ARRAY_BUFFER, num_vertices*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    //Load Vertex Element Objects
    glGenBuffers(1, &eao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
    printf("No. of elements: %d\n", num_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_elements*sizeof(GLuint), elements, GL_STATIC_DRAW);

    GLint status;
    //Loading vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if(status!=GL_TRUE)
    {
        fputs("Unable to compile vertex shader\n", stderr);

        printf("Your GLSL version is %s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        char buffer[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
        printf("%s\n", buffer);
        return -1;
    }

    //Load fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_shader_source, NULL);
    glCompileShader(fragment_shader);


    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if(status!=GL_TRUE)
    {
        fputs("Unable to compile fragment shader\n", stderr);
        char buffer[512];
        printf("Your GLSL version is %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        glGetShaderInfoLog(fragment_shader, 512, NULL, buffer);
        printf("%s\n", buffer);
        return -1;
    }
    //Link and compile shader program
    GLuint shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    //Bind fragment output to output 0
    glBindFragDataLocation(shader_program, 0, "outColor");

    //Link and use the shader program
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

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
    int width, height;
    unsigned char* pixels = loadBMP("chess.bmp", &width, &height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    //Deallocate memory used up by the image file
    unload_BMP(pixels);


    //Set position attribute
    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, (7*sizeof(GLfloat)), 0); //Now stores in the VAO vao
    glEnableVertexAttribArray(posAttrib);

    //Set color attributes for the board
    GLint colAttrib = glGetAttribLocation(shader_program, "color");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colAttrib);
    glfwSwapInterval(1);

    //Set texture attributes for the board
    GLuint texAttrib = glGetAttribLocation(shader_program, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(texAttrib);

    //Enable alpha ( transparency )
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    prev_selected = selected;
    prev_game_state = engine->GetGameStatus();
    current_player = engine->GetCurrentPlayer();

    double prev_event_time = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //Asks the engine if there is any change in the game.
        //And if a change in the vertice information is required
        //if(selected_x || prev_game_state != engine->GetGameStatus() || current_player != engine->GetCurrentPlayer())

        glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * sizeof(GLfloat), vertices);
        LoadPieceTextures();
        LoadBoardColor();

        prev_selected = selected;
        prev_game_state = engine->GetGameStatus();
        current_player = engine->GetCurrentPlayer();



        glClear(GL_COLOR_BUFFER_BIT);
        DrawBoard();

        int game_status = engine->GetGameStatus();

        bool stop_game = false;
        //Get status of the game
        //Game is stopped if there's either a checkmate or stalemate
        switch(game_status)
        {
        case GAME_CHECKMATE:
            stop_game = true;
            if(engine->GetCurrentPlayer()==1)
                printf("Checkmate. Player 2 wins. \n");
            else
                printf("Checkmate. Player 1 wins. \n");
            break;
        case GAME_STALEMATE:
            stop_game = true;
            printf("Stalemate. Draw.\n");
            break;
        }
        if(stop_game)
            glfwSetWindowShouldClose(window, GL_TRUE);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        while(glfwGetTime()-prev_event_time<EVENT_PROCESS_WAIT_TIME)
            Sleep(1);
        prev_event_time = glfwGetTime();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    return 1;
}

void Chess2D::DrawBoard()
{
    glDrawElements(GL_TRIANGLES, 8*8*6, GL_UNSIGNED_INT, 0);
}
Chess2D::~Chess2D()
{
    delete[] vertices;
    delete[] elements;
    delete[] vertex_source;
    delete[] fragment_source;
}

void Chess2D::LoadBoardColor()
{
    static moves prev_ai_move = moves(-2,-2,-2,-2);
    moves ai_move;
    int index = 0;
    if(single_player_mode)
        ai_move = engine->GetAIMove();


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
    if(action ==GLFW_PRESS && engine->GetGameStatus()!=GAME_THINKING)
    {
        double x,y;
        glfwGetCursorPos(window, &x, &y);

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
                    engine->ProcessInput(prev_x, prev_y, square_x, square_y);
                }
            }
        }
    }
}

#endif // _CHESS2D_H
