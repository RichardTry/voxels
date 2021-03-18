#include "program.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

Program::Program()
{
    prog = glCreateProgram();
}

Program::~Program()
{

}

void Program::AddShader(char* filename, GLuint type)
{
    static int len = 8000;
    char* text = (char*)malloc(len);
    memset(text, 0, len);
    FILE* f;
    f = fopen(filename, "rb");
    if (!f)
    {
        std::cout << "error: " << filename << ": no such file or directory" << std::endl;
        return;
    }
    fread(text, 1, len, f);
    fclose(f);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &text, NULL);
    glCompileShader(shader);
    free(text);

    GLint success;
    GLchar log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cout << filename << ": " << log;
        return;
    }

    glAttachShader(prog, shader);
    glLinkProgram(prog);

    glDeleteShader(shader);

    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(prog, 512, NULL, log);
        std::cout << log << std::endl;
        return;
    }
}

void Program::Use()
{
    glUseProgram(prog);
}
