// Local Headers
#include "glitter.hpp"
#include "Shader.hpp"
#include "util.hpp" // GLCall Macro
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cassert>

struct Cube
{
    Cube(float posx, float posy, float posz, float op, float or , float oy, float s)
    {
        x = posx;
        y = posy;
        z = posz;
        pitch = op;
        roll = or;
        yaw = oy;
        sX = sY = sZ = s;
    };

    float x;
    float y;
    float z;
    float pitch;
    float roll;
    float yaw;
    float sX, sY, sZ;
};


int main(int argc, char * argv[])
{

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 uniform_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float model_position[3] = { 0.0f, 0.0f, 0.0f };
    float model_scale = 50.0f;
    
    std::vector<Cube> cubes;

    float model_roll = 0.0f;
    float model_pitch = 0.0f;
    float model_yaw = 0.0f;

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
    
    GLFWwindow* mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", nullptr, nullptr);
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


    {   // Scope to make sure all buffers get desroyed before
        // glfwTerminate is called
    
        // Vertex initialization
        
        float vertices[] = {
            //front
            -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // 0 
             1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // 1
             1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // 2
            -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // 3
            // right
             1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // 4
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // 5
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // 6
             1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // 7
            // back
             1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // 8
             -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // 9
             -1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // 10
             1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // 11
            // left
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // 12 
            -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // 13
            -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // 14
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // 15
            // bottom
            -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, // 16
             1.0f, -1.0f,  1.0f, 1.0f, 1.0f, // 17
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // 18
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // 19
            // top 
            -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, // 20
             1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // 21
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // 22
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f  // 23
        };

        unsigned int indices[] = {
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            4, 5, 6,
            6, 7, 4,
            // back
            8, 9, 10,
            10, 11, 8,
            // left
            12, 13, 14,
            14, 15, 12,
            // bottom
            18, 19, 16,
            16, 17, 18,
            // top
            20, 21, 22,
            22, 23, 20
        };

        cubes.push_back(Cube(0.0f, 0.0f, 0.0f, 10.0f, 2.0f, 3.0f, 50.0f) );
        cubes.push_back(Cube(10.0f, 40.0f, 0.0f, 10.0f, 2.0f, 3.0f, 50.0f));
        cubes.push_back(Cube(-10.0f, 0.0f, -10.0f, 10.0f, 2.0f, 3.0f, 50.0f));
        cubes.push_back(Cube(0.0f, 20.0f, 100.0f, 10.0f, 2.0f, 3.0f, 50.0f));
        cubes.push_back(Cube(-50.0f, 20.0f, 50.0f, 10.0f, 2.0f, 3.0f, 50.0f));

        
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLCall(glEnable(GL_DEPTH_TEST));

        // Init vertexArray
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // 4 * 4 * sizeof(float)
        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);
        va.addBuffer(vb, layout);
    
       

        // Init Index buffer object
        IndexBuffer ib(indices, sizeof(indices));

        // Create shader
        Shader basicShader("../RTG_proj/Shaders/basic.shader");
        basicShader.bind();
        // set shader uniforms
        basicShader.SetUniform4f("u_Color", uniform_color.x, uniform_color.y, uniform_color.z, uniform_color.w);
        basicShader.setUniformMat4f("u_MVP", glm::mat4(1.0f));
        
        // Create Texture
        Texture texture("../RTG_PROJ/Resources/test.png");
        texture.bind();
        basicShader.SetUniform1i("u_Texture", 0);
        // Unbind everything
        va.unbind();
        vb.unbind();
        ib.unbind();
        basicShader.unbind();

        Renderer renderer;
 
        while (glfwWindowShouldClose(mWindow) == false)
        {
            if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) // User presses esc
                glfwSetWindowShouldClose(mWindow, true);

            

            // Create transforms
            glm::mat4 view(1.0f);
            glm::mat4 proj(1.0f);
            
            //proj = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 10.0f);
            proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
            
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -300.0f));
            //view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            
            // DRAW LOGIC HERE /////////////
            renderer.setClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            renderer.clear();

            basicShader.bind();
            va.bind();
            ib.bind();
            
             // update uniforms
             basicShader.SetUniform4f("u_Color", uniform_color.x, uniform_color.y, uniform_color.z, uniform_color.w);
            
             for (auto cube : cubes)
            {
                 glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(cube.x, cube.y, cube.z));
                 model = glm::scale(model, glm::vec3(cube.sX));
                 model = glm::rotate(model, glm::radians(cube.roll), glm::vec3(0.0f, 0.0f, 1.0f));
                 model = glm::rotate(model, glm::radians(cube.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
                 model = glm::rotate(model, glm::radians(cube.yaw),  glm::vec3(0.0f, 1.0f, 0.0f));

                 glm::mat4 mvp = proj * view * model;

                 basicShader.setUniformMat4f("u_MVP", mvp);
                 renderer.draw(va, ib, basicShader); // Render second model
            }
        
            ImGui_ImplGlfwGL3_NewFrame();
            // IMgui Rendering and input
            {
                ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
                ImGui::SliderFloat("roll", &model_roll, -360.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f  
                ImGui::SliderFloat("pitch", &model_pitch, -360.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f  
                ImGui::SliderFloat("yaw", &model_yaw, -360.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f  
                ImGui::SliderFloat2("Aposition", model_position, -1000.0f, 1000.0f);
                ImGui::SliderFloat("A-Zpos", &model_position[2], -100.0f, 100.0f);
                ImGui::SliderFloat("Scale", &model_scale, 0.10f, 100.0f);





                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
                ImGui::ColorEdit4("uniform color", (float*)&uniform_color); // Edit 3 floats representing a color

                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        
            // Render with imgui
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            }

            // Flip Buffers and Draw
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }
    }
    // Cleanup
    
    glfwTerminate();
    


    return EXIT_SUCCESS;
}

