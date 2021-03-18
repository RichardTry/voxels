#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <vector>

class VAO
{
    private:
        GLuint vao;
        std::vector<GLuint> vbos;
        GLuint ebo = 0;
        size_t ebo_len;

    public:
        VAO();
        ~VAO();

        void Bind();
        void AddVBO(const std::vector<float>& data);
        void AddEBO(const std::vector<unsigned>& data);

        void Draw();
};

#endif // VAO_H
