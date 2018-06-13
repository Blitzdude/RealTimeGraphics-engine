/* RealTimeGraphics-Project demo */
/*
    Controls:
    - W,A,S,D -> Move
    - Use mouse to Turn camera
    - Press space to toggle Mouse cursor and camera 
*/

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
// Changes

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

#include "tests/testClearColor.hpp"

// Method Prototypes 
// ---------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

// Global - camera
Camera camera(glm::vec3(-200.0f, 0.0f, 00.0f));

float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
bool enableInput = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


struct Cube
{
    Cube(float posx, float posy, float posz, float op, float or , float oy, float sx, float sy, float sz)
    {
        position[0] = posx;
        position[1] = posy;
        position[2] = posz;

        orientation[0] = op;
        orientation[1] = or;
        orientation[2] = oy;

        scale[0] = sx;
        scale[1] = sy;
        scale[2] = sz;
    };
    float position[3];
    float orientation[3];
    float scale[3];
};

struct Material {
    std::string name;
    glm::vec3 ambient ;
    glm::vec3 diffuse ;
    glm::vec3 specular;
    float shininess;
};

struct Light {
    ImVec4 color;
    glm::vec3 ambient ;
    glm::vec3 diffuse ;
    glm::vec3 specular;
};

int main(int argc, char * argv[])
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 light_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float lightIntensity = 1.0f;

    // Create and initialize the light
    Light light;
    light.color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    light.ambient = glm::vec3(0.2f, 0.20f, 0.20f );
    light.diffuse = glm::vec3(0.50f, 0.50f, 0.50f );
    light.specular = glm::vec3( 0.50f, 0.50f, 0.50f );

    // Load GLFW and Create a Window
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL();
    // set up input callbacks


    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // tell GLFW to capture our mouse

    // Create imgui context
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(mWindow, true);


    {
        // Scope to make sure all buffers get destroyed before
        // glfwTerminate is called. Otherwise GLbuffers wouldn't be destroyed 
        // after glfwTermination

        // Cube vertex initialization
        float vertices[] = {
            /* position.xyz, normals.xyz, uv.st*/
            //front
            -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0 
            1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 1
            1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 2
            -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 3
            // right
            1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 4
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 5
            1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 6
            1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 7
            // back
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 8
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // 9
            -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // 10
            1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // 11
            // left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12 
            -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 13
            -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 14
            -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 15
             // bottom
             -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // 16
             1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // 17
             1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 18
             -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 19
             // top 
             -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 20
             1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 21
             1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 22
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 23
        };

        // cube vertices
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

        // Create shaders
        // --------------------------------
        Shader basicShader("Shaders/basic.shader");

        // Load Textures
        // ---------------------------------
        Texture texture("Resources/test.png");

        // Init Cube values
        // ----------------------------------
        Cube texturedCube1(0.0f, 20.0f, -45.0f, 0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 10.0f);
        Cube texturedCube2(-75.0f, -14.5f, -25.0f, 0.0f, 0.0f, 0.0f, 160.0f, 5.0f, 220.0f);
        Cube lightCube(-100.0f, 50.0f, -10.0f, 0.0f, 0.0f, 0.0f, 2.5f, 2.5f, 2.5f);

        // Enable GL parameteres
        // ---------------------------------
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLCall(glEnable(GL_DEPTH_TEST));

        // Init cube vertexArrayObject
        // -------------------------------
        VertexArray cubeVao;
        VertexBuffer cubeVbo(vertices, sizeof(vertices)); // 4 * 4 * sizeof(float)
        VertexBufferLayout layout;
        layout.push<float>(3); // vertex.xyz
        layout.push<float>(3); // normal.xyz
        layout.push<float>(2); // uv.st
        cubeVao.addBuffer(cubeVbo, layout);
        
        // Init Index buffer object for cube
        IndexBuffer ib(indices, sizeof(indices));
        
        // Unbind everything
        cubeVao.unbind();
        cubeVbo.unbind();
        texture.unbind();
        ib.unbind();
        basicShader.unbind();

        // Create Renderer
        // ---------------------------
        Renderer renderer;

        test::TestClearColor test;

        while (glfwWindowShouldClose(mWindow) == false)
        {
            renderer.clear();

            // Per frame time logic
            // ----------------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            test.OnUpdate(deltaTime);
            test.OnRender();

            // input
            // -----------------------------------
            processInput(mWindow);

            // IMgui Rendering and input
            ImGui_ImplGlfwGL3_NewFrame();
            {
             
                test.OnImGuiRender();
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
    static int oldState = GLFW_RELEASE;
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

    int newState = glfwGetKey(window, GLFW_KEY_SPACE);
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS)
    {
        if (enableInput) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            enableInput = false;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            enableInput = true;
        }
    }
    oldState = newState;
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

    if (enableInput)
        camera.ProcessMouseMovement(xoffset, yoffset);
}
