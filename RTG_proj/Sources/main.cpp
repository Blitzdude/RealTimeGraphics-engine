// Local Headers
#include "glitter.hpp"
#include "Shader.hpp"
#include "util.hpp" // GLCall Macro
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, -3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


struct Cube
{
    Cube(float posx, float posy, float posz, float op, float or , float oy, float s)
    {
        position[0] = posx;
        position[1] = posy;
        position[2] = posz;

        pitch = op;
        roll = or;
        yaw = oy;
        sX = sY = sZ = s;
    };
    float position[3];
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
    ImVec4 light_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    float model_position[3] = { 0.0f, 0.0f, 0.0f };
    float model_scale = 50.0f;
    

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
    
    // Make Context current and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);

    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    gladLoadGL();
    // set up input callbacks


    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // tell GLFW to capture our mouse

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

        Cube texturedCube(0.0f, 0.0f, -45.0f, 0.0f, 24.0f, 40.0f, 5.0f);
        Cube lightCube(10.0f, 10.0f, -10.0f, 0.0f, 0.0f, 0.0f, 2.5f);
        
        // Enable GL parameteres
        // ---------------------------------
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLCall(glEnable(GL_DEPTH_TEST));

        // Init cube vertexArray
        VertexArray cubeVao;
        VertexBuffer cubeVbo(vertices, sizeof(vertices)); // 4 * 4 * sizeof(float)
        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);
        cubeVao.addBuffer(cubeVbo, layout);

        // Init Index buffer object for cube
        IndexBuffer ib(indices, sizeof(indices));


        // Create textured cube shader
        Shader basicShader("../RTG_proj/Shaders/basic.shader");
        basicShader.bind();


        // set shader uniforms
        basicShader.SetUniform4f("u_Color", uniform_color.x, uniform_color.y, uniform_color.z, uniform_color.w);
        basicShader.setUniformMat4f("u_MVP", glm::mat4(1.0f));

        
        // Create Texture
        Texture texture("../RTG_PROJ/Resources/test.png");
        texture.bind();
        basicShader.SetUniform1i("u_Texture", 0);
        basicShader.unbind();
        // Create light shader
        Shader lightShader("../RTG_proj/Shaders/light.shader");
        lightShader.bind();
        // Unbind everything
        cubeVao.unbind();
        cubeVbo.unbind();
        texture.unbind();
        ib.unbind();
        basicShader.unbind();

        Renderer renderer;
 
        while (glfwWindowShouldClose(mWindow) == false)
        {
            // Per frame time logic
            // ----------------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // input
            // -----------------------------------
            processInput(mWindow);

            //proj = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 10.0f);
            glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
            glm::mat4 view = camera.GetViewMatrix();
            
            // DRAW LOGIC HERE /////////////
            renderer.setClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            renderer.clear();

            cubeVao.bind();
            ib.bind();
            
            // Render textured cube
            // ------------------------
            {
                basicShader.bind();
                // update uniforms
                basicShader.SetUniform4f("u_Color", uniform_color.x, uniform_color.y, uniform_color.z, uniform_color.w);
                basicShader.SetUniform3f("u_LightColor", light_color.x, light_color.y, light_color.z);

                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(texturedCube.position[0], texturedCube.position[1], texturedCube.position[2]));
                model = glm::scale(model, glm::vec3(texturedCube.sX));
                model = glm::rotate(model, glm::radians(texturedCube.roll), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, glm::radians(texturedCube.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(texturedCube.yaw), glm::vec3(0.0f, 1.0f, 0.0f));

                glm::mat4 mvp = proj * view * model;
                texture.bind();
                basicShader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(cubeVao, ib, basicShader); // Render model
                texture.unbind();
                basicShader.unbind();
            }


            // Render light cube
            // ------------------------
            {
                lightShader.bind();
                // update uniforms
                lightShader.SetUniform4f("u_Color", light_color.x, light_color.y, light_color.z, light_color.w);

                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(lightCube.position[0], lightCube.position[1], lightCube.position[2]));
                model = glm::scale(model, glm::vec3(lightCube.sX));
                model = glm::rotate(model, glm::radians(lightCube.roll), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, glm::radians(lightCube.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(lightCube.yaw), glm::vec3(0.0f, 1.0f, 0.0f));

                glm::mat4 mvp = proj * view * model;
                lightShader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(cubeVao, ib, lightShader); // Render model
            }


        
            ImGui_ImplGlfwGL3_NewFrame();
            // IMgui Rendering and input
            {
                ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)

                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
                ImGui::ColorEdit4("uniform color", (float*)&uniform_color); // Edit 3 floats representing a color
                ImGui::ColorEdit4("Light color", (float*)&light_color); // Edit 3 floats representing a color
                ImGui::SliderFloat3("Ligth Position", lightCube.position, -100.0f, 100.0f);
                

                double xPos;
                double yPos;
                glfwGetCursorPos(mWindow, &xPos, &yPos);
                ImGui::Text("MousePos: %.1f %.1f )", (float)xPos, (float)yPos);
                ImGui::Text("MouseScroll: %.1f )", camera.Zoom);


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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    static bool toggle = true;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!toggle)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            toggle = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            toggle = false;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}