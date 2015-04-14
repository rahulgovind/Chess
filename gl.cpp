#include "gl.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cstring>
#include "image_loader.h"

#include <iostream>
using namespace std;


const char *text_vertex_shader =  "#version 150\n"
                        "in vec2 pos;\n"
                        "in vec2 texcoord;\n"
                        "out vec2 Texcoord;\n"
                        "void main()\n"
                        "{\n"
                        "Texcoord = texcoord;\n"
                        "gl_Position = vec4(pos, 0, 1);\n"
                        "}\n";

const char *text_fragment_shader =    "#version 150\n"
                            "in vec2 Texcoord;\n"
                            "uniform sampler2D tex;\n"
                            "out vec4 outColor;\n"
                            "void main()\n"
                            "{\n"
                            "vec4 temp = texture(tex, Texcoord);\n"
                            "if(temp[0] == 0 && temp[1] == 0 && temp[2] == 0)\n"
                            "outColor = vec4(0,0,0,0);\n"
                            "else\n"
                            "outColor = temp;\n"
                            "}\n";

const char* imagemenu_vertex_shader =   "#version 150\n"
                                        "in vec2 pos;\n"
                                        "in vec2 texcoord;\n"
                                        "out vec2 Texcoord;\n"
                                        "void main()\n"
                                        "{\n"
                                        "Texcoord = texcoord;\n"
                                        "gl_Position = vec4(pos, 0, 1);\n"
                                        "}\n";

const char *imagemenu_fragment_shader =    "#version 150\n"
                            "in vec2 Texcoord;\n"
                            "uniform sampler2D tex;\n"
                            "out vec4 outColor;\n"
                            "void main()\n"
                            "{\n"
                            "vec4 temp = texture(tex, Texcoord);\n"
                            "outColor = temp;\n"
                            "}\n";

Shader::Shader(string vertex_source, string fragment_source)
{
    int status;

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source_temp = vertex_source.c_str();
    glShaderSource(vert_shader, 1, &vertex_source_temp, 0);
    glCompileShader(vert_shader);

    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if(status!=GL_TRUE)
    {
        char buffer[512];
        glGetShaderInfoLog(vert_shader, 512, NULL, buffer);
        throw ShaderException((const char*)buffer);
    }

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* frag_source_temp = fragment_source.c_str();
    glShaderSource(frag_shader, 1, &frag_source_temp, 0);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
    if(status!=GL_TRUE)
    {
        char buffer[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, buffer);
        throw ShaderException((const char*)buffer);
    }

    id = glCreateProgram();
    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);

    glLinkProgram(id);
}

void Shader::Load()
{
    glBindFragDataLocation(id, 0, "outColor");
    glUseProgram(id);

}

Shader::~Shader()
{
    glDeleteProgram(id);
    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);
}


GLSave::GLSave()
{
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&program);
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&active_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&vbo);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&eao);

    glGetBooleanv(GL_BLEND, (GLboolean*)&blend);
    glGetBooleanv(GL_DEPTH_TEST, (GLboolean*)&depth);
    glGetBooleanv(GL_STENCIL_TEST, (GLboolean*)&stencil);
}

GLSave::~GLSave()
{
    if(vao!=0) glBindVertexArray(vao);
    if(vbo!=0) glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if(eao!=0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_BINDING, eao);

    if(program!=0) glUseProgram(program);
    if(active_texture!=0) glActiveTexture(active_texture);
    (blend == true)? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    (depth == true)? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    (stencil==true)? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

Text::Text()
{
    GLSave save;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

    glGenVertexArrays(1, &text_vao);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &tex_buffer);

    glBindVertexArray(text_vao);
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &text_vertex_shader, NULL);
    glCompileShader(vert);

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &text_fragment_shader, NULL);
    glCompileShader(frag);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert);
    glAttachShader(shader_program, frag);

    glBindFragDataLocation(shader_program, 0, "outColor");

    tex_pixels = loadBMP("images/ExportedFont.bmp", &tex_width, &tex_height);
    glGenTextures(1, &texture);
}

void Text::Draw(float x, float y, float size, string s)
{
    GLSave save;


    glBindVertexArray(text_vao);
    vector<glm::vec2> vertices;
    vector<glm::vec2> texcoord;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    for(unsigned int i=0;i<s.length();i++)
    {
        float r = 0.3f;

        float tex_x = (s[i]%16)/16.0f;
        float tex_y = (s[i]/16)/16.0f;



        vertices.push_back(glm::vec2(x, y));
        vertices.push_back(glm::vec2(x+size*r,y));
        vertices.push_back(glm::vec2(x+size*r,y-size));

        vertices.push_back(glm::vec2(x, y));
        vertices.push_back(glm::vec2(x+size*r, y-size));
        vertices.push_back(glm::vec2(x, y-size));



        texcoord.push_back(glm::vec2(tex_x,             1.0f - tex_y));
        texcoord.push_back(glm::vec2(tex_x+1.0f/16.0f*r,  1.0f - tex_y));
        texcoord.push_back(glm::vec2(tex_x+1.0f/16.0f*r,  1.0f - (tex_y + 1.0f/16.0f)));

        texcoord.push_back(glm::vec2(tex_x,             1.0f - tex_y));
        texcoord.push_back(glm::vec2(tex_x+1.0f/16.0f*r,  1.0f - (tex_y + 1.0f/16.0f)));
        texcoord.push_back(glm::vec2(tex_x           ,  1.0f - (tex_y + 1.0f/16.0f)));

        x+=(size*r);

    }

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tex_buffer);
    glBufferData(GL_ARRAY_BUFFER, texcoord.size()*sizeof(glm::vec2), &texcoord[0], GL_STATIC_DRAW);

    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    glActiveTexture(GL_TEXTURE0+texture-1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_pixels);



    int texAttrib = glGetAttribLocation(shader_program, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, tex_buffer);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


    unsigned int posAttrib = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(posAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    unsigned int texSampler = glGetUniformLocation(shader_program, "tex");
    glUniform1i(texSampler, texture-1);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glDisableVertexAttribArray(posAttrib);
    glDisableVertexAttribArray(texAttrib);

}

Text::~Text()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &tex_buffer);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &text_vao);
    glDeleteProgram(shader_program);
    glDeleteShader(vert);
    glDeleteShader(frag);
}


ImageMenu::ImageMenu(string filename, int window_width, int window_height, Rect<int> Boundary, vector<Rect<int> > Option_coords)
{



    GLSave save;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &tex_buffer);
    shader = new Shader(imagemenu_vertex_shader, imagemenu_fragment_shader);

    glGenTextures(1, &texture);
    tex_pixels = loadBMP(filename.c_str(), &tex_width, &tex_height);

    float scale_factor_x = (float)Boundary.width/tex_width;
    float scale_factor_y = (float)Boundary.height/tex_height;

    //Translate Option_coords relative to Boundary and scale to size of image
    for(vector<Rect<int> >::iterator it = Option_coords.begin();it!=Option_coords.end();it++)
    {
        it->left += Boundary.left;
        it->top += Boundary.top;

        //Scale to correct size;
        it->left *= scale_factor_x;
        it->top *= scale_factor_y;
        it->width *= scale_factor_x;
        it->height *= scale_factor_y;

        Rect<float> temp;
        temp.left = 2*(float)it->left/window_width - 1.0;
        temp.top = 1.0 - 2*(float)it->top/window_height;
        temp.width = 2*(float)it->width/window_width;
        temp.height = 2*(float)it->height/window_height;
        option_coords.push_back(temp);
    }

    boundary.left = 2*(float)Boundary.left/window_width - 1.0;
    boundary.top = 1.0 - (float)Boundary.top/window_height;
    boundary.width = 2*(float)Boundary.width/window_width;
    boundary.height = 2*(float)Boundary.height/window_height;

}

void ImageMenu::DrawMenu()
{
    GLSave save;

    glDisable(GL_BLEND);
    glBindVertexArray(vao);
    vector<glm::vec2> vertices;
    vector<glm::vec2> texcoords;

    vertices.push_back(glm::vec2(boundary.left, boundary.top));
    vertices.push_back(glm::vec2(boundary.left + boundary.width, boundary.top));
    vertices.push_back(glm::vec2(boundary.left + boundary.width, boundary.top - boundary.height));

    vertices.push_back(glm::vec2(boundary.left, boundary.top));
    vertices.push_back(glm::vec2(boundary.left + boundary.width, boundary.top - boundary.height));
    vertices.push_back(glm::vec2(boundary.left, boundary.top - boundary.height));

    texcoords.push_back(glm::vec2(0.0f, 1.0f));
    texcoords.push_back(glm::vec2(1.0f, 1.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));

    texcoords.push_back(glm::vec2(0.0f, 1.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));
    texcoords.push_back(glm::vec2(0.0f, 0.0f));

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tex_buffer);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size()*sizeof(glm::vec2), &texcoords[0], GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0 + texture - 1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_pixels);

    shader->Load();

    int texAttrib = glGetAttribLocation(shader->id, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, tex_buffer);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


    unsigned int posAttrib = glGetAttribLocation(shader->id, "pos");
    glEnableVertexAttribArray(posAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    unsigned int texSampler = glGetUniformLocation(shader->id, "tex");
    glUniform1i(texSampler, texture-1);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glDisableVertexAttribArray(posAttrib);
    glDisableVertexAttribArray(texAttrib);

}

int ImageMenu::ProcessInput(float x, float y)
{

    int i = 1;

    for(vector<Rect<float> >::iterator it = option_coords.begin();it!=option_coords.end();it++)
    {

        if(x>(it->left) && x<(it->left + it->width) && y<(it->top) && y>(it->top - it->height))
        {
            return i;
        }
        i++;
    }

    return 0;
}

ImageMenu::~ImageMenu()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &tex_buffer);
    glDeleteVertexArrays(1, &vao);

    delete shader;
    delete tex_pixels;
}
