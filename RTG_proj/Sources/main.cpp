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
#include "tests/test.hpp"
#include "tests/testClearColor.hpp"
#include "tests/testMaterialAndLighting.hpp"
#include "tests/testTexture2D.hpp"

// Method Prototypes 
// ---------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Globals
test::Test* currentTest = nullptr;

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
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION) );
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// Scope to make sure all buffers get destroyed before
		// glfwTerminate is called. Otherwise GLbuffers wouldn't be destroyed 
		// after glfwTermination
	 
		// Create Renderer
		Renderer renderer;
		
		// Create imgui context
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(mWindow, true);
		ImGui::StyleColorsDark();
		
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;
		/// TODO: Command line arguments to go to certain test
		testMenu->RegisterTest<test::TestClearColor>("Clear Color Test");
		testMenu->RegisterTest<test::TestTexture2D>("Texture2D test");
		testMenu->RegisterTest<test::TestMaterialAndLighting>("Materials and Lighting test");

		
		glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
		glfwSetCursorPosCallback(mWindow, mouse_callback);
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		
		while (glfwWindowShouldClose(mWindow) == false)
		{
			// No renderer created, so no clearing going on FIXME!!!!
			// Per frame time logic
			// ----------------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			renderer.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			renderer.clear();
			
			ImGui_ImplGlfwGL3_NewFrame();
			if (currentTest)
			{
				currentTest->OnUpdate(deltaTime);
				currentTest->KeyboardInput(mWindow);
				currentTest->OnRender();
				ImGui::Begin("Testing");
				if (currentTest != testMenu && ImGui::Button("Back"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

		  // Render with imgui
		  ImGui::Render();
		  ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	      // Flip Buffers and Draw
	      glfwSwapBuffers(mWindow);
	      glfwPollEvents();
		}


    // Cleanup
	delete currentTest;
	if (currentTest != testMenu)
		delete testMenu;
	}

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