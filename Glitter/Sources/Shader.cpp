#include "Shader.hpp"


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
    glDeleteProgram(m_programId);
}

void Shader::use()
{
    glUseProgram(m_programId);
}

void Shader::unuse()
{
    glUseProgram(0);
}

ShaderSource Shader::parseShader(const char * filepath)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMNET = 1
    };

    std::ifstream stream(filepath);

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
                type = ShaderType::VERTEX;

            }
            else
            {
                ss[(int)type] << line << '\n';
            }
        }
    }

    // Return the struct
    return { ss[0].str(), ss[1].str() };
}

GLuint Shader::compileShader(GLuint type, const std::string & source)
{

    GLuint id = glCreateShader(type);

    if (id == 0) // Error check: shader create status
    {
        fprintf(stderr, "Error Creating shader type %d\n", type);
        assert(0);
    }

    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // if given nullptr for length, src is assumed null terminated
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile " <<
        (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;

}

void Shader::createShaderProgram()
{
    GLuint program = glCreateProgram();

    ShaderSource sSource = parseShader(m_filepath.c_str());

    GLuint vs = compileShader(GL_VERTEX_SHADER, sSource.vertexSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, sSource.fragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);


}
