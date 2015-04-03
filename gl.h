#ifndef _XGL_H
#define _XGL_H

#include <cstring>
#include <iostream>
#include <exception>


using namespace std;

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

#endif // _GL_H
