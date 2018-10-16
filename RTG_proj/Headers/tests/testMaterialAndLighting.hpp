#pragma once

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Renderer.hpp"

#include "glm\glm.hpp"
#include "test.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "glitter.hpp"
#include "imgui.h"
#include "Model.hpp"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"


namespace test
{

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
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Light {
	ImVec4 color;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};


class TestMaterialAndLighting : public Test
{
public:
	TestMaterialAndLighting();
	~TestMaterialAndLighting();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;
	void MouseInput(double xpos, double ypos) override;
	void KeyboardInput(GLFWwindow *window);
	
	Camera m_camera;
	
private:
	// Timing
	float m_deltaTime = 0.0f;

	Renderer m_renderer;

	static float m_vertices[24*8];
	static unsigned int m_indices[12*3];

	std::vector<Material> m_materials;
	Shader m_basicShader;
	Shader m_meshShader;
	Shader m_lightShader;
	
	// Cubes
	Cube m_texturedCube1;
	Cube m_texturedCube2;
	Cube m_lightCube;
	// Textures
	Texture m_texture;
	Texture m_blank;
	Texture m_treasureDiff;
	Light m_light;

	// cube vertex arrays
	VertexArray m_cubeVao;
	VertexBuffer m_cubeVbo;
	IndexBuffer m_ib;

	Model m_treasure;

	float m_lastX = WINDOW_WIDTH / 2.0f;
	float m_lastY = WINDOW_HEIGHT / 2.0f;
	bool m_firstMouse = true;
	bool m_enableInput = true;


	ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4 m_light_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	float m_lightIntensity = 1.0f;

	// Matrices
	glm::mat4 proj;
	glm::mat4 view;
};
}