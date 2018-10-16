#include "testTexture2D.hpp"
#include "util.hpp"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace test {
TestTexture2D::TestTexture2D()
	: m_clearColor{ 0.2f, 0.3f, 0.8f, 1.0f },
	m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
{
	float positions[] = {
		-50.0f, -50.0f, 0.0f, 0.0f, // 0
		 50.0f, -50.0f, 1.0f, 0.0f, // 1
		 50.0f,  50.0f, 1.0f, 1.0f, // 2
		-50.0f,  50.0f, 0.0f, 1.0f // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	

	// Enable GL blend functions
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	m_shader = std::make_unique<Shader>("Shaders/textureBasic.shader"); 
	m_texture = std::make_unique<Texture>("Resources/bbface.png");
	m_va = std::make_unique<VertexArray>();
	m_vb = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

	// create vertex buffer
	// Create buffer layout
	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);

	m_va->addBuffer(*m_vb, layout);
	// initialize index buffer
	m_ib = std::make_unique<IndexBuffer>(indices, 6);

	// Bind texture to shader
	m_shader->bind();
	m_texture->bind();
	m_shader->SetUniform1i("u_texture", 0); 
	
	// Unbind everything
	m_va->unbind();
	m_vb->unbind();
	m_ib->unbind();
	m_shader->unbind();

}
TestTexture2D::~TestTexture2D()
{
}

void TestTexture2D::OnUpdate(float deltaTime)
{
}

void TestTexture2D::OnRender()
{

	// temprorary
	GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_texture->bind();
	m_shader->bind();
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
		glm::mat4 mvp = m_proj * m_view * model;
		m_shader->setUniformMat4f("u_MVP", mvp);
		m_renderer.draw(*m_va, *m_ib, *m_shader);
	}
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
		glm::mat4 mvp = m_proj * m_view * model;
		m_shader->setUniformMat4f("u_MVP", mvp);
		m_renderer.draw(*m_va, *m_ib, *m_shader);
	}

}
void TestTexture2D::OnImGuiRender()
{
	ImGui::ColorEdit4("Clear color", m_clearColor);
	ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);
	ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
	ImGui::Text("Application avertage %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

}