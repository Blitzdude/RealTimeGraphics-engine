#pragma once 

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <unordered_map>



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

    void bind() const;
    void unbind() const;

    // Uniform setters
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string& name, glm::mat4& matrix);

private:
    ShaderSource parseShader(const char* filepath);
    GLuint compileShader(GLuint type, const std::string& source);
    void createShaderProgram();
    GLint GetUniformLocation(const std::string& name);
private:
    
    std::unordered_map<std::string, int> m_uniformLocationCache;
    std::string m_filepath;
    GLuint m_programId;
};
