#include "Shader.hpp"
#include "util.hpp"


#include <fstream>
#include <sstream>
#include <assert.h>

Shader::Shader(std::string filepath) 
    : m_filepath(filepath), m_programId(0)
{
    createShaderProgram();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_programId));
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_programId));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string & name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));

}

void Shader::SetUniform1f(const std::string & name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string & name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));

}

void Shader::setUniformMat4f(const std::string & name, glm::mat4 & matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

ShaderSource Shader::parseShader(const char * filepath)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    if (stream.fail())
        std::cout << "file not Found: " << filepath << std::endl;
    
    else
        std::cout << "Reading from file: " << filepath << std::endl;
    
    
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode fragment
                type = ShaderType::FRAGMENT;

            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
            
    }
    
    return { ss[0].str(), ss[1].str() };
}

GLint Shader::GetUniformLocation(const std::string & name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];


    GLCall(GLint location = glGetUniformLocation(m_programId, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform " << name << " doesn't exist" << std::endl;

    m_uniformLocationCache[name] = location;
    return location;
}

GLuint Shader::compileShader(GLuint type, const std::string & source)
{

    GLCall(GLuint id = glCreateShader(type));

    if (id == 0) // Error check: shader create status
    {
        fprintf(stderr, "Error Creating shader type %d\n", type);
        assert(0);
    }

    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr)); // if given nullptr for length, src is assumed null terminated
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

    std::cout << "Failed to compile " <<
        (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;

}

void Shader::createShaderProgram()
{
    GLCall(GLuint program = glCreateProgram());

    ShaderSource sSource = Shader::parseShader(m_filepath.c_str());
    
    GLuint vs = compileShader(GL_VERTEX_SHADER, sSource.vertexSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, sSource.fragmentSource);

     GLCall(glAttachShader(program, vs));
     GLCall(glAttachShader(program, fs));
     GLCall(glLinkProgram(program));
    
    // Check that the program is valid
    GLCall(glValidateProgram(program));
    GLint status;
    GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &status));
    if(status == GL_FALSE)
    {
        std::cout << "Program not valid" << std::endl;
        GLCall(glDeleteProgram(program));
    }
    m_programId = program;
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
}
