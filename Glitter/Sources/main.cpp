// Local Headers
#include "glitter.hpp"
#include "Shader.hpp"


// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cassert>


// Define an error save macro

#define GLCall(x) GLClearError();\
x;\
assert(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]" << std::hex << error << " " << function <<
        " " << file << ":" << line;
        return false;
    }
    return true;
}

static GLuint d_compileShader(GLuint type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        // TODO: GLCall macro prob not needed in this scope
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
            << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint d_createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = d_compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = d_compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    
    GLint status;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &status));
    if ( status == GL_FALSE)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(program, length, &length, message);
        std::cout << "Failed to link Program " << std::endl;
        std::cout << message << std::endl;
        
        glDeleteProgram(program);
    }
    
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int main(int argc, char * argv[])
{

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 uniform_color = ImVec4(0.1f, 0.2f, 0.8f, 1.0f);

    // Load GLFW and Create a Window
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);
    if (!mWindow)
    {
        glfwTerminate();
        return -1;
    }

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }
    
    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // Create imgui context
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(mWindow, true);



    // Vertex initialization

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int VAO;
    GLCall(glGenVertexArrays(1,&VAO));
    GLCall(glBindVertexArray(VAO));
    
    unsigned int VBO;
    // Init Vertex buffer
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
    
    unsigned int IBO;
    GLCall(glGenBuffers(1, &IBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
    
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    GLCall(glEnableVertexAttribArray(0));
    
    // Create shader
    ShaderSource source = Shader::parseShader("../../../Glitter/Shaders/basic.shader");
    std::cout << "VERTEX:" << source.vertexSource << std::endl;
    std::cout << "FRAGMENT:" << source.fragmentSource << std::endl;
    unsigned int basicProgram = d_createShader(source.vertexSource, source.fragmentSource);
    
 
    
    while (glfwWindowShouldClose(mWindow) == false)
    {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) // User presses esc
            glfwSetWindowShouldClose(mWindow, true);

        static float f = 0.0f;
        
        GLCall(glUseProgram(basicProgram));
        GLCall(int location = glGetUniformLocation(basicProgram, "u_Color"));
        assert(location != -1);
        GLCall(glUniform4f(location, uniform_color.x, uniform_color.y, uniform_color.z, uniform_color.w));

        // Background Fill Color
        GLCall(glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        GLCall(glUseProgram(0));
        ImGui_ImplGlfwGL3_NewFrame();

        {
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::ColorEdit4("uniform color", (float*)&uniform_color); // Edit 3 floats representing a color

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        
        
        // Render with imgui
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    
    GLCall(glDeleteProgram(basicProgram));
    glfwTerminate();
    

    // Cleanup

    return EXIT_SUCCESS;
}
