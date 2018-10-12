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

// Tests
#include "tests/testClearColor.hpp"
#include "tests/testMaterialAndLighting.hpp"

// Method Prototypes 
// ---------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Globals
test::Test* currentTest;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(int argc, char * argv[])
{
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
	
    gladLoadGL();
    // set up input callbacks


    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION) );

    // tell GLFW to capture our mouse

    // Create imgui context
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(mWindow, true);


    {
        // Scope to make sure all buffers get destroyed before
        // glfwTerminate is called. Otherwise GLbuffers wouldn't be destroyed 
        // after glfwTermination

        // Create Renderer
        // ---------------------------
        //Renderer renderer;

        //test::TestClearColor test;
		test::TestMaterialAndLighting test;

		currentTest = &test;
		glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
		glfwSetCursorPosCallback(mWindow, mouse_callback);
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


        while (glfwWindowShouldClose(mWindow) == false)
        {

            // Per frame time logic
            // ----------------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            test.OnUpdate(deltaTime);
			test.processInput(mWindow);
            test.OnRender();

            // input
            // -----------------------------------

            // IMgui Rendering and input
            ImGui_ImplGlfwGL3_NewFrame();
            {
                test.OnImGuiRender();
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
	currentTest->MouseInput(xpos, ypos);

}