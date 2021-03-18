#include "windowlib.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

WindowLib::WindowLib()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Could not initialize GLFW");
    }
}

WindowLib::~WindowLib()
{
    glfwTerminate();
}
