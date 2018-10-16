#include "testClearColor.hpp"
#include "util.hpp"
#include "imgui.h"

namespace test {
TestClearColor::TestClearColor()
    : m_clearColor {0.2f, 0.3f, 0.8f, 1.0f}
{
	// Enable GL blend functions

	// Create Renderer

	// Create Imgui Context
}
TestClearColor::~TestClearColor()
{
}

void TestClearColor::OnUpdate(float deltaTime)
{
}

void TestClearColor::OnRender()
{
    GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void TestClearColor::OnImGuiRender()
{
    ImGui::ColorEdit4("Clear color", m_clearColor);
}

}