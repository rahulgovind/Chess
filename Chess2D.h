#ifndef _CHESS2D_H
#define _CHESS2D_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/glfw3.h>
#include <cstdio>
#include <bits/stdc++.h>

#include "image_loader.h"
#include "engine.h"
#include "evaluation.h"
#include "shader_source.h"

using namespace std;

struct RGB
{
    float r;
    float g;
    float b;

    RGB() {};

    RGB(float R, float G, float B) { r = R; b = B; g = G; }
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

    static RGB white;
    static RGB black;
    static RGB selected_color;
    static RGB highlighted_color;

    static Engine *engine;
    void LoadBoardColor();
    void LoadPieceTextures();
    void DrawBoard();
    static void ProcessMouseInput(GLFWwindow*,int,int,int);



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
GLfloat* Chess2D::vertices = NULL;
GLuint Chess2D::num_vertices = 0;

Engine* Chess2D::engine = NULL;

RGB Chess2D::white(0.8,0.8,0.8);
RGB Chess2D::black(0.4, 0.4, 0.4);
RGB Chess2D::selected_color(0.1, 0.8, 0.1);
RGB Chess2D::highlighted_color(0.3, 1.0, 0.3);

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
    engine = new Engine;
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(window_width, window_height, "Chess 2D", NULL, NULL);
    if(!window)
    {
        fputs("Unable to create window\n", stderr);
        return -1;
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
        fputs(vertex_source, stderr);

        fputs("Unable to compile vertex shader\n", stderr);
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

    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //Asks the engine if there is any change in the game.
        //And if a change in the vertice information is required
        if(engine->CheckIfUpdated())
        {
            LoadPieceTextures();
            LoadBoardColor();
            glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * sizeof(GLfloat), vertices);
        }


        glClear(GL_COLOR_BUFFER_BIT);
        DrawBoard();

        int game_status = engine->GetGameStatus();

        bool stop_game = false;
        //Get statatus of the game
        //Game is stopped if there's either a checkmate or stalemate
        switch(game_status)
        {
        case CHECKMATE_PLAYER2:
            stop_game = true;
            printf("Checkmate. Player 1 wins\n");
            break;
        case CHECKMATE_PLAYER1:
            stop_game = true;
            printf("Checkmate. Player 2 wins\n");
            break;
        case STALEMATE:
            stop_game = true;
            printf("Stalemate. Draw.\n");
            break;
        }
        if(stop_game)
            glfwSetWindowShouldClose(window, GL_TRUE);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
    int index = 0;
    for(int j=0;j<8;j++)
        for(int i = 0;i<8;i++)
        {

            int color = engine->GetColor(i,j);

            RGB board_color;


            if(color==0)
                if((i+j)%2 == 0)
                    board_color = white;
                else
                    board_color = black;
            else if(color == 1)
                board_color = selected_color;
            else
                board_color = highlighted_color;

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
    if(action ==GLFW_PRESS)
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
            printf("x: %d\ty: %d\n", square_x, square_y);
            engine->ProcessInput(square_x, square_y);
        }
    }
}




#endif // _CHESS2D_H
