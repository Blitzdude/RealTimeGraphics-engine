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
#include "Model.hpp"
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
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
        orientation[1] = or ;
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

    Light light;
    light.color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    light.ambient = glm::vec3(0.2f, 0.20f, 0.20f );
    light.diffuse = glm::vec3(0.50f, 0.50f, 0.50f );
    light.specular = glm::vec3( 0.50f, 0.50f, 0.50f );

    float model_position[3] = { 0.0f, 0.0f, 0.0f };
    float model_scale = 50.0f;


    float model_roll = 0.0f;
    float model_pitch = 0.0f;
    float model_yaw = 0.0f;

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

        // Create material container
        std::vector<Material> materials;
        materials.push_back({ "Gold",   { 0.24725f, 0.1995f, 0.0745f }, { 0.75164f, 0.60648f, 0.22648f }, { 0.628281f, 0.555802f, 0.366065f }, 0.4f * 128 });
        materials.push_back({ "Silver", { 0.19225f, 0.19225f, 0.19225f },{ 0.50754f, 0.50754f, 0.50754f },{ 0.508273f, 0.508273f, 0.508273f }, 0.4f * 128 });
        materials.push_back({"Bronze"  ,{ 0.2125f, 0.1275f, 0.054f },{ 0.714f, 0.4284f, 0.18144f },{ 0.393548f, 0.271906f, 0.166721f }, 0.2f * 128 });
        materials.push_back({ "Brass"   ,{ 0.24725f, 0.1995f, 0.0745f },{ 0.75164f, 0.60648f, 0.22648f },{ 0.628281f, 0.555802f, 0.366065f }, 0.21794872f * 128 });
        materials.push_back({"Copper"        ,{ 0.0f, 0.0f, 0.0f },{ 0.7038f, 0.27048f, 0.0828f },{ 0.256777f, 0.137622f, 0.086014f }, 0.1f * 128});
        materials.push_back({"Black_Plastic" ,{ 0.0f, 0.0f, 0.0f },{ 0.01f, 0.01f, 0.0f },{ 0.5f, 0.5f, 0.5f }, 0.25f * 128 });
        materials.push_back({"Cyan_Plastic" ,{ 0.0f, 0.1f, 0.06f },{ 0.0f, 0.50980392f, 0.50980392f },{ 0.50196078f, 0.50196078f, 0.50196078f }, 0.25f * 128 });
        materials.push_back({"Green_Plastic",{ 0.0f, 0.0f, 0.0f },{ 0.1f, 0.35f, 0.1f },{ 0.45f, 0.55f, 0.45f }, 0.25f * 128 });
        materials.push_back({"White_Plastic",{ 0.0f, 0.0f, 0.0f },{ 0.55f, 0.55f, 0.55f },{ 0.7f, 0.7f, 0.7f }, 0.25f * 128 });
        materials.push_back({"Black_Rubber"  ,{ 0.02f, 0.02f, 0.02f },{ 0.01f, 0.01f, 0.01f },{ 0.4f, 0.4f, 0.4f }, 0.078125f * 128 });


        // Vertex initialization
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


        // Load Models and init cubes
        // -----------------------------------
        Shader meshShader("../RTG_proj/Shaders/mesh.shader");
        Model treasure("../RTG_proj/Resources/Models/TreasureChest/treasure_chest.obj");

        // Create shaders
        // --------------------------------
        Shader basicShader("../RTG_proj/Shaders/basic.shader");
        Shader lightShader("../RTG_proj/Shaders/light.shader");

        // Load Textures
        // ---------------------------------
        Texture texture("../RTG_PROJ/Resources/test.png");
        Texture blank("../RTG_PROJ/Resources/blank.png");
        Texture treasureDiff("../RTG_proj/Resources/Models/TreasureChest/Treasurechest_DIFF.png");

        Cube texturedCube1(0.0f, 20.0f, -45.0f, 0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 10.0f);
        Cube texturedCube2(-25.0f, -14.5f, 0.0f, 0.0f, 0.0f, 0.0f, 160.0f, 5.0f, 200.0f);
        Cube lightCube(50.0f, 50.0f, -10.0f, 0.0f, 0.0f, 0.0f, 2.5f, 2.5f, 2.5f);

        // Enable GL parameteres
        // ---------------------------------
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLCall(glEnable(GL_DEPTH_TEST));

        // Init cube vertexArray
        VertexArray cubeVao;
        VertexBuffer cubeVbo(vertices, sizeof(vertices)); // 4 * 4 * sizeof(float)
        VertexBufferLayout layout;
        layout.push<float>(3); // vertex.xyz
        layout.push<float>(3); // normal.xyz
        layout.push<float>(2); // uv.st
        cubeVao.addBuffer(cubeVbo, layout);
        
        // Init Index buffer object for cube
        IndexBuffer ib(indices, sizeof(indices));
        
        // Create textured cube shader
        
        // Unbind everything
        
        cubeVao.unbind();
        cubeVbo.unbind();
        texture.unbind();
        ib.unbind();
        basicShader.unbind();
        meshShader.unbind();


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

            // projection and view matrix
            // -----------------------------------
            glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.f, 10000.0f);
            glm::mat4 view = camera.GetViewMatrix();

            // Draw logic begin
            // -----------------------------------
            renderer.setClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            renderer.clear();

            // Render treasure model
            // ---------------------------
            meshShader.bind();
            treasureDiff.bind();
            for (int i = 0; i < materials.size(); i++)
            {
                // Texture uniform
                meshShader.SetUniform1i("texture_diffuse1", 0);
                // Material uniforms
                meshShader.SetUniform3f("u_Material.ambient", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
                meshShader.SetUniform3f("u_Material.diffuse", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
                meshShader.SetUniform3f("u_Material.specular", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
                meshShader.SetUniform1f("u_Material.shininess", materials[i].shininess);
                // Light uniforms
                meshShader.SetUniform3f("u_ViewPos", camera.Position.x, camera.Position.y, camera.Position.z);
                meshShader.SetUniform3f("u_LightColor", light_color.x, light_color.y, light_color.z);
                meshShader.SetUniform3f("u_Light.position", lightCube.position[0], lightCube.position[1], lightCube.position[2]);
                meshShader.SetUniform3f("u_Light.ambient", lightIntensity *light.ambient.x, lightIntensity *light.ambient.y, lightIntensity * light.ambient.z);
                meshShader.SetUniform3f("u_Light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
                meshShader.SetUniform3f("u_Light.specular", light.specular.x, light.specular.y, light.specular.z);
                // mvp matrix
                glm::mat4 model(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, (40.0f * i) - 200.0f));
                model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 mvp = proj * view * model;
                // model uniforms
                meshShader.setUniformMat4f("u_ModelViewProj", mvp);
                meshShader.setUniformMat4f("u_Model", model);
                // draw
                treasure.Draw(meshShader);
            }
             meshShader.unbind();
            
            // Render textured cube1
            // ------------------------
            
            {
            basicShader.bind();
            // update uniforms
            basicShader.SetUniform1i("u_Texture", 0);
            basicShader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

            basicShader.SetUniform3f("u_Material.ambient", 1.0f, 0.5f, 0.31f);
            basicShader.SetUniform3f("u_Material.diffuse", 1.0f, 0.5f, 0.31f);
            basicShader.SetUniform3f("u_Material.specular", 0.5f, 0.5f, 0.5f);
            basicShader.SetUniform1f("u_Material.shininess", 32.0f);

            basicShader.SetUniform3f("u_ViewPos", camera.Position.x, camera.Position.y, camera.Position.z);
            basicShader.SetUniform3f("u_Light.position", lightCube.position[0], lightCube.position[1], lightCube.position[2]);
            basicShader.SetUniform3f("u_Light.ambient", 0.2f, 0.2f, 0.2f);
            basicShader.SetUniform3f("u_Light.diffuse", 0.5f, 0.5f, 0.5f);
            basicShader.SetUniform3f("u_Light.specular", 1.f, 1.0f, 1.0f);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(texturedCube1.position[0], texturedCube1.position[1], texturedCube1.position[2]));
            model = glm::rotate(model, glm::radians(texturedCube1.orientation[0]), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(texturedCube1.orientation[1]), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(texturedCube1.orientation[2]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(texturedCube1.scale[0], texturedCube1.scale[1], texturedCube1.scale[2]));

            glm::mat4 mvp = proj * view * model;
            texture.bind();
            basicShader.setUniformMat4f("u_ModelViewProj", mvp);
            basicShader.setUniformMat4f("u_Model", model);
            renderer.draw(cubeVao, ib, basicShader); // Render model
            texture.unbind();
            }
            
            // Render textured cube2
            // ------------------------
            
            {
            // update uniforms
            blank.bind(1);
            basicShader.SetUniform1i("u_Texture", 1);
            basicShader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

            meshShader.SetUniform3f("u_Material.ambient", materials[8].ambient[0], materials[8].ambient[1], materials[8].ambient[2]);
            meshShader.SetUniform3f("u_Material.diffuse", materials[8].diffuse[0], materials[8].diffuse[1], materials[8].diffuse[2]);
            meshShader.SetUniform3f("u_Material.specular", materials[8].specular[0], materials[8].specular[1], materials[8].specular[2]);
            meshShader.SetUniform1f("u_Material.shininess", materials[8].shininess);

            basicShader.SetUniform3f("u_ViewPos", camera.Position.x, camera.Position.y, camera.Position.z);
            basicShader.SetUniform3f("u_LightColor", light_color.x, light_color.y, light_color.z);
            basicShader.SetUniform3f("u_Light.position", lightCube.position[0], lightCube.position[1], lightCube.position[2]);
            basicShader.SetUniform3f("u_Light.ambient", 0.2f, 0.2f, 0.2f);
            basicShader.SetUniform3f("u_Light.diffuse", 0.5f, 0.5f, 0.5f);
            basicShader.SetUniform3f("u_Light.specular", 1.f, 1.0f, 1.0f);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(texturedCube2.position[0], texturedCube2.position[1], texturedCube2.position[2]));
            model = glm::rotate(model, glm::radians(texturedCube2.orientation[0]), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(texturedCube2.orientation[1]), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(texturedCube2.orientation[2]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(texturedCube2.scale[0], texturedCube2.scale[1], texturedCube2.scale[2]));

            glm::mat4 mvp = proj * view * model;
           
            basicShader.setUniformMat4f("u_ModelViewProj", mvp);
            basicShader.setUniformMat4f("u_Model", model);
            renderer.draw(cubeVao, ib, basicShader); // Render model
            blank.unbind();
            basicShader.unbind();
            }
            
            // Render light cube
            // ------------------------
            
            {
            lightShader.bind();
            // update uniforms
            lightShader.SetUniform4f("u_Color", light_color.x, light_color.y, light_color.z, light_color.w);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(lightCube.position[0], lightCube.position[1], lightCube.position[2]));
            model = glm::rotate(model, glm::radians(lightCube.orientation[0]), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(lightCube.orientation[1]), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(lightCube.orientation[2]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(lightCube.scale[0], lightCube.scale[1], lightCube.scale[2]));

            glm::mat4 mvp = proj * view * model;
            lightShader.setUniformMat4f("u_ModelViewProj", mvp);
            renderer.draw(cubeVao, ib, lightShader); // Render model
            lightShader.unbind();
            }
            
            
            
            ImGui_ImplGlfwGL3_NewFrame();
            // IMgui Rendering and input
            {
                ImGui::Text("Debug");  // Display some text (you can use a format string too)

                ImGui::ColorEdit3("clear color", (float*)&clear_color);         // Edit 3 floats representing a color
                ImGui::ColorEdit4("Light color", (float*)&light_color);          // Edit 3 floats representing a color
                ImGui::SliderFloat3("Ligth Position", lightCube.position, -100.0f, 100.0f);
                ImGui::SliderFloat3("Ligth Ambient", (float*)&light.ambient, 0.0f, 1.0f);
                ImGui::SliderFloat3("Ligth Diffuse", (float*)&light.diffuse, 0.0f, 1.0f);
                ImGui::SliderFloat3("Ligth Specular", (float*)&light.specular, 0.0f, 1.0f);
                ImGui::SliderFloat("Light intensity", &lightIntensity, 0.1f, 100.0f);

                 // CUBE1 -------------------------------
                ImGui::SliderFloat3("cube1 Position", texturedCube1.position, -100.0f, 100.0f);
                ImGui::SliderFloat3("cube1 Roll/Pitch/Yaw", texturedCube1.orientation, -360.0f, 360.0f);
                ImGui::SliderFloat3("cube1 Scale", texturedCube1.scale, -0.0f, 100.0f);
                
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}