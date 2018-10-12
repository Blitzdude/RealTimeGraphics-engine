#include "testMaterialAndLighting.hpp"

#include "util.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

namespace test
{
float TestMaterialAndLighting::m_vertices[] =
{
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

unsigned int TestMaterialAndLighting::m_indices[] =
{
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

TestMaterialAndLighting::TestMaterialAndLighting()
	: m_basicShader("Shaders/basic.shader"), 
	m_lightShader("Shaders/light.shader"),
	m_meshShader("Shaders/mesh.shader"),
	// Camera
    m_camera(glm::vec3(-200.0f, 100.0f, 0.0f)),
	// Cubes
    m_texturedCube1(0.0f, 20.0f, -45.0f, 0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 10.0f),
    m_texturedCube2(-75.0f, -14.5f, -25.0f, 0.0f, 0.0f, 0.0f, 160.0f, 5.0f, 220.0f),
    m_lightCube(-100.0f, 50.0f, -10.0f, 0.0f, 0.0f, 0.0f, 2.5f, 2.5f, 2.5f),
	// Textures
    m_texture("Resources/test.png"),
	m_blank("Resources/blank.png"),
	m_treasureDiff("Resources/Models/TreasureChest/Treasurechest_DIFF.png"),

	m_treasure("Resources/Models/TreasureChest/treasure_chest.obj")
{
	// Set Light Cube variables
	m_light.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_light.ambient = glm::vec3(0.2f, 0.20f, 0.20f);
	m_light.diffuse = glm::vec3(0.50f, 0.50f, 0.50f);
	m_light.specular = glm::vec3(0.50f, 0.50f, 0.50f);

	// populate Materials container
	m_materials.push_back({ "Gold",   { 0.24725f, 0.1995f, 0.0745f }, { 0.75164f, 0.60648f, 0.22648f }, { 0.628281f, 0.555802f, 0.366065f }, 0.4f * 128 });
	m_materials.push_back({ "Silver", { 0.19225f, 0.19225f, 0.19225f },{ 0.50754f, 0.50754f, 0.50754f },{ 0.508273f, 0.508273f, 0.508273f }, 0.4f * 128 });
	m_materials.push_back({ "Bronze"  ,{ 0.2125f, 0.1275f, 0.054f },{ 0.714f, 0.4284f, 0.18144f },{ 0.393548f, 0.271906f, 0.166721f }, 0.2f * 128 });
	m_materials.push_back({ "Brass"   ,{ 0.24725f, 0.1995f, 0.0745f },{ 0.75164f, 0.60648f, 0.22648f },{ 0.628281f, 0.555802f, 0.366065f }, 0.21794872f * 128 });
	m_materials.push_back({ "Copper"        ,{ 0.01f, 0.01f, 0.01f },{ 0.7038f, 0.27048f, 0.0828f },{ 0.256777f, 0.137622f, 0.086014f }, 0.1f * 128 });
	m_materials.push_back({ "Black_Plastic" ,{ 0.01f, 0.01f, 0.01f },{ 0.01f, 0.01f, 0.0f },{ 0.5f, 0.5f, 0.5f }, 0.25f * 128 });
	m_materials.push_back({ "Cyan_Plastic" ,{ 0.01f, 0.1f, 0.06f },{ 0.0f, 0.50980392f, 0.50980392f },{ 0.50196078f, 0.50196078f, 0.50196078f }, 0.25f * 128 });
	m_materials.push_back({ "Green_Plastic",{ 0.01f, 0.01f, 0.01f },{ 0.1f, 0.35f, 0.1f },{ 0.45f, 0.55f, 0.45f }, 0.25f * 128 });
	m_materials.push_back({ "White_Plastic",{ 0.01f, 0.01f, 0.01f },{ 0.55f, 0.55f, 0.55f },{ 0.7f, 0.7f, 0.7f }, 0.25f * 128 });
	m_materials.push_back({ "Black_Rubber"  ,{ 0.02f, 0.02f, 0.02f },{ 0.01f, 0.01f, 0.01f },{ 0.4f, 0.4f, 0.4f }, 0.078125f * 128 });


	// Enable GL parameteres
	   // ---------------------------------
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GLCall(glEnable(GL_DEPTH_TEST));

	// Load Models and Init cubes


	// Init cube vertexArrayObject
	// -------------------------------
	m_cubeVao;
	m_cubeVbo.init(m_vertices, sizeof(m_vertices)); // 4 * 4 * sizeof(float)
	VertexBufferLayout layout;
	layout.push<float>(3); // vertex.xyz
	layout.push<float>(3); // normal.xyz
	layout.push<float>(2); // uv.st
	m_cubeVao.addBuffer(m_cubeVbo, layout);

	// Init Index buffer object for cubes
	m_ib.init(m_indices, sizeof(m_indices));

	// Unbind everything
	m_cubeVao.unbind();
	m_cubeVbo.unbind();
	m_texture.unbind();
	m_ib.unbind();
	m_basicShader.unbind();
}

TestMaterialAndLighting::~TestMaterialAndLighting()
{
	
}

void TestMaterialAndLighting::OnUpdate(float deltaTime)
{
	// Delta timing
	m_deltaTime = deltaTime;
	// projection and view matrix
	// -----------------------------------
	proj = glm::perspective(glm::radians(m_camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.f, 10000.0f);
	view = m_camera.GetViewMatrix();
}

void TestMaterialAndLighting::OnRender()
{
	m_renderer.setClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);
	m_renderer.clear();
	// Render treasure models
	// ---------------------------
	m_meshShader.bind();
	m_treasureDiff.bind();
	for (int i = 0; i < m_materials.size(); i++)
	{
		// Texture uniform
		m_meshShader.SetUniform1i("texture_diffuse1", 0);
		// Material uniforms
		m_meshShader.SetUniform3f("u_Material.ambient",   m_materials[i].ambient[0],  m_materials[i].ambient[1],  m_materials[i].ambient[2]);
		m_meshShader.SetUniform3f("u_Material.diffuse",   m_materials[i].diffuse[0],  m_materials[i].diffuse[1],  m_materials[i].diffuse[2]);
		m_meshShader.SetUniform3f("u_Material.specular",  m_materials[i].specular[0], m_materials[i].specular[1], m_materials[i].specular[2]);
		m_meshShader.SetUniform1f("u_Material.shininess", m_materials[i].shininess);
		// Light uniforms
		m_meshShader.SetUniform3f("u_ViewPos",	      m_camera.Position.x, m_camera.Position.y,			m_camera.Position.z);
		m_meshShader.SetUniform3f("u_LightColor",     m_light_color.x,     m_light_color.y,				m_light_color.z);
		m_meshShader.SetUniform3f("u_Light.position", m_lightCube.position[0], m_lightCube.position[1], m_lightCube.position[2]);
		m_meshShader.SetUniform3f("u_Light.ambient",  m_lightIntensity   *m_light.ambient.x,  m_lightIntensity *m_light.ambient.y, m_lightIntensity * m_light.ambient.z);
		m_meshShader.SetUniform3f("u_Light.diffuse",  m_light.diffuse.x,  m_light.diffuse.y,  m_light.diffuse.z);
		m_meshShader.SetUniform3f("u_Light.specular", m_light.specular.x, m_light.specular.y, m_light.specular.z);
		// mvp matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (40.0f * i) - 200.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mvp = proj * view * model;
		// model uniforms
		m_meshShader.setUniformMat4f("u_ModelViewProj", mvp);
		m_meshShader.setUniformMat4f("u_Model", model);
		// draw
		m_treasure.Draw(m_meshShader);
	}
	m_meshShader.unbind();

	// Render textured cube1
	// ------------------------

	{
		m_basicShader.bind();
		// update uniforms
		m_basicShader.SetUniform1i("u_Texture", 0);
		m_basicShader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

		m_basicShader.SetUniform3f("u_Material.ambient", 0.20f, 0.2f, 0.2f);
		m_basicShader.SetUniform3f("u_Material.diffuse", 1.0f, 1.0f, 1.0f);
		m_basicShader.SetUniform3f("u_Material.specular", 0.5f, 0.5f, 0.5f);
		m_basicShader.SetUniform1f("u_Material.shininess", 32.0f);

		m_basicShader.SetUniform3f("u_ViewPos",		   m_camera.Position.x, m_camera.Position.y, m_camera.Position.z);
		m_basicShader.SetUniform3f("u_Light.position", m_lightCube.position[0], m_lightCube.position[1], m_lightCube.position[2]);
		m_basicShader.SetUniform3f("u_Light.ambient",  m_lightIntensity *m_light.ambient.x, m_lightIntensity *m_light.ambient.y, m_lightIntensity *m_light.ambient.z);
		m_basicShader.SetUniform3f("u_Light.diffuse",  m_light.diffuse.x,  m_light.diffuse.y,  m_light.diffuse.z);
		m_basicShader.SetUniform3f("u_Light.specular", m_light.specular.x, m_light.specular.y, m_light.specular.z);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_texturedCube1.position[0], m_texturedCube1.position[1], m_texturedCube1.position[2]));
		model = glm::rotate(model, glm::radians(m_texturedCube1.orientation[0]), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(m_texturedCube1.orientation[1]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_texturedCube1.orientation[2]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(m_texturedCube1.scale[0], m_texturedCube1.scale[1], m_texturedCube1.scale[2]));

		glm::mat4 mvp = proj * view * model;
		m_texture.bind();
		m_basicShader.setUniformMat4f("u_ModelViewProj", mvp);
		m_basicShader.setUniformMat4f("u_Model", model);
		m_renderer.draw(m_cubeVao, m_ib, m_basicShader); // Render model
		m_texture.unbind();
	}

	// Render textured cube2
	// ------------------------

	{
		// update uniforms
		m_blank.bind(0);
		m_basicShader.SetUniform1i("u_Texture", 0);
		m_basicShader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

		m_basicShader.SetUniform3f("u_Material.ambient", m_materials[8].ambient[0], m_materials[8].ambient[1], m_materials[8].ambient[2]);
		m_basicShader.SetUniform3f("u_Material.diffuse", m_materials[8].diffuse[0], m_materials[8].diffuse[1], m_materials[8].diffuse[2]);
		m_basicShader.SetUniform3f("u_Material.specular", m_materials[8].specular[0], m_materials[8].specular[1], m_materials[8].specular[2]);
		m_basicShader.SetUniform1f("u_Material.shininess", m_materials[8].shininess);

		m_basicShader.SetUniform3f("u_ViewPos", m_camera.Position.x, m_camera.Position.y, m_camera.Position.z);
		m_basicShader.SetUniform3f("u_LightColor", m_light_color.x,  m_light_color.y,	  m_light_color.z);
		m_basicShader.SetUniform3f("u_Light.position", m_lightCube.position[0], m_lightCube.position[1], m_lightCube.position[2]);
		m_basicShader.SetUniform3f("u_Light.ambient",  m_lightIntensity *m_light.ambient.x,    m_lightIntensity *m_light.ambient.y, m_lightIntensity * m_light.ambient.z);
		m_basicShader.SetUniform3f("u_Light.diffuse",  m_light.diffuse.x, m_light.diffuse.y,   m_light.diffuse.z);
		m_basicShader.SetUniform3f("u_Light.specular", m_light.specular.x, m_light.specular.y, m_light.specular.z);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_texturedCube2.position[0], m_texturedCube2.position[1], m_texturedCube2.position[2]));
		model = glm::rotate(model, glm::radians(m_texturedCube2.orientation[0]), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(m_texturedCube2.orientation[1]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_texturedCube2.orientation[2]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(m_texturedCube2.scale[0], m_texturedCube2.scale[1], m_texturedCube2.scale[2]));

		glm::mat4 mvp = proj * view * model;

		m_basicShader.setUniformMat4f("u_ModelViewProj", mvp);
		m_basicShader.setUniformMat4f("u_Model", model);
		m_renderer.draw(m_cubeVao, m_ib, m_basicShader); // Render model
		m_blank.unbind();
		m_basicShader.unbind();
	}

	// Render light cube
	// ------------------------     
	{
		m_lightShader.bind();
		// update uniforms
		m_lightShader.SetUniform4f("u_Color", m_light_color.x, m_light_color.y, m_light_color.z, m_light_color.w);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_lightCube.position[0], m_lightCube.position[1], m_lightCube.position[2]));
		model = glm::rotate(model, glm::radians(m_lightCube.orientation[0]), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(m_lightCube.orientation[1]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_lightCube.orientation[2]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(m_lightCube.scale[0], m_lightCube.scale[1], m_lightCube.scale[2]));

		glm::mat4 mvp = proj * view * model;
		m_lightShader.setUniformMat4f("u_ModelViewProj", mvp);
		m_renderer.draw(m_cubeVao, m_ib, m_lightShader); // Render model
		m_lightShader.unbind();
	}
}

void TestMaterialAndLighting::OnImGuiRender()
{
	ImGui::Text("Debug");  // Display some text (you can use a format string too)

	ImGui::ColorEdit3("clear color", (float*)&m_clear_color);         // Edit 3 floats representing a color
	ImGui::ColorEdit4("Light color", (float*)&m_light_color);          // Edit 3 floats representing a color
	ImGui::SliderFloat3("Ligth Position", m_lightCube.position, -200.0f, 200.0f);
	ImGui::SliderFloat3("Ligth Ambient", (float*)&m_light.ambient, 0.0f, 1.0f);
	ImGui::SliderFloat3("Ligth Diffuse", (float*)&m_light.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat3("Ligth Specular", (float*)&m_light.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("Light intensity", &m_lightIntensity, 0.1f, 100.0f);

	// CUBE1 -------------------------------
	ImGui::SliderFloat3("cube1 Position", m_texturedCube1.position, -200.0f, 200.0f);
	ImGui::SliderFloat3("cube1 Roll/Pitch/Yaw", m_texturedCube1.orientation, -360.0f, 360.0f);
	ImGui::SliderFloat3("cube1 Scale", m_texturedCube1.scale, 0.0f, 100.0f);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("RealTimeGraphics-Course Demo by Joel Känsälä:");
	ImGui::Text("WASD to move");
	ImGui::Text("Mouse to Rotate camera");
	ImGui::Text("Space to Toggle Mouse");
	ImGui::Text("Esc to quit");

	// Render with imgui
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void TestMaterialAndLighting::processInput(GLFWwindow *window)
{
	static int oldState = GLFW_RELEASE;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera.ProcessKeyboard(FORWARD, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera.ProcessKeyboard(BACKWARD, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera.ProcessKeyboard(LEFT, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera.ProcessKeyboard(RIGHT, m_deltaTime);

	int newState = glfwGetKey(window, GLFW_KEY_SPACE);
	if (newState == GLFW_RELEASE && oldState == GLFW_PRESS)
	{
		if (m_enableInput) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_enableInput = false;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_enableInput = true;
		}
	}
	oldState = newState;
}

void TestMaterialAndLighting::MouseInput(double xpos, double ypos)
{

	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xoffset = xpos - m_lastX;
	float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top

	m_lastX = xpos;
	m_lastY = ypos;

	if (m_enableInput)
		m_camera.ProcessMouseMovement(xoffset, yoffset);
}

}
