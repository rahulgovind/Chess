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

#endif // _GL_H
