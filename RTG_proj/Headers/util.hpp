#pragma once
#include "glitter.hpp"

// Define an error save macro
#if _DEBUG
    #define GLCall(x) GLClearError();\
    x;\
    assert(GLLogCall())
#else
    #define GLCall(x) x
#endif // DEBUG

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]0x" << std::hex << error << " " << std::endl;
        return false;
    }
    return true;
}
