#pragma once

#include <memory>

#include "test.hpp"
#include "glm/glm.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"


namespace test {

class TestTexture2D : public Test
{
public:
	TestTexture2D();
	~TestTexture2D();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;
private:
	Renderer m_renderer;
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Texture> m_texture;

	std::unique_ptr<VertexArray> m_va;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<IndexBuffer> m_ib;

	float m_clearColor[4];


	glm::vec3 m_translationA = { 200.0f, 200.0f, 0.0f };
	glm::vec3 m_translationB = { 400.0f, 200.0f, 0.0f };

	glm::mat4 m_proj;
	glm::mat4 m_view;

};
}