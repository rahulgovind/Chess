#include "gl.h"
#define GLEW_STATIC
#include <GL/glew.h>

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
