#ifndef PROGRAM_H
#define PROGRAM_H

#include <glad/glad.h>

class Program
{
    private:
        GLuint prog;

    public:
        Program();
        ~Program();

        void AddShader(char* filename, GLuint type);
        void Use();
};

#endif // PROGRAM_H
