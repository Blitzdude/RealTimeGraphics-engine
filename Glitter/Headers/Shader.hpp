#pragma once 

#include <glad/glad.h>

#include <string>
#include <iostream>



struct ShaderSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
public:
    Shader(std::string filepath);
    ~Shader();

    void use();
    void unuse();

private:
    ShaderSource parseShader(const char* filepath);
    GLuint compileShader(GLuint type, const std::string& source);
    void createShaderProgram();

    std::string m_filepath;
    GLuint m_programId;
};
