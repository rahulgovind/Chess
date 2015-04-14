#ifndef _XGL_H
#define _XGL_H

#include <cstring>
#include <iostream>
#include <exception>
#include <vector>

using namespace std;

template<class T>
struct Point
{
    T x;
    T y;
};

template<class T>
struct Rect
{
    T left;
    T top;
    union{T width; T right; };
    union{T height; T bottom; };
};

class Shader
{
    unsigned int vert_shader;
    unsigned int frag_shader;
public:
    unsigned int id;
    Shader() = default;
    Shader(string,string);
    void Load();
    ~Shader();
};

class ShaderException: public exception
{
  private:
      string msg;
  public:
      ShaderException(string m="Shader Exception"): msg(m) {}
      ~ShaderException() throw() {}
      const char* what() const throw() { return msg.c_str(); }
};

class GLSave
{
private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int eao;
    unsigned int program;
    unsigned int active_texture;
    bool blend;
    bool depth;
    bool stencil;

public:
    GLSave();
    ~GLSave();

};

class Text
{
private:
    unsigned int text_vao;
    unsigned int vertex_buffer;
    unsigned int tex_buffer;
    unsigned int texture;
    unsigned char *tex_pixels;
    int tex_width;
    int tex_height;
    unsigned int shader_program;
    unsigned int vert;
    unsigned int frag;
public:
    Text();
    void Draw(float x, float y, float size, string s);
    ~Text();
};

class ImageMenu
{
private:
    unsigned int vao;
    unsigned int vertex_buffer;
    unsigned int tex_buffer;
    unsigned char *tex_pixels;
    int tex_width;
    int tex_height;

    unsigned int texture;
    Shader *shader;
    Rect<float> boundary;
    vector<Rect<float> > option_coords;
public:
    ImageMenu(string, int window_width, int window_height, Rect<int> Boundary, vector<Rect<int> > Option_coords);
    void DrawMenu(bool translucent = false);
    int ProcessInput(float x, float y);
    ~ImageMenu();
};
#endif // _GL_H
