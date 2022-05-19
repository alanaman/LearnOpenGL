#include "glerror_handler.hpp"


void GLClearError()
{
    // std::cout<<"ok"<<std::endl;
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error](" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void assertCall(const char* file, int line)
{

    std::cout << "[ASSERT error] "<< file << ":" << line << std::endl;
    return;
}