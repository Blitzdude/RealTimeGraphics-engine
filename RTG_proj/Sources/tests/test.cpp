#include "test.hpp"
#include "imgui.h"
namespace test
{
TestMenu::TestMenu(Test *& currenTestPtr)
	: m_currentTest(currenTestPtr)
{
}
TestMenu::~TestMenu()
{
}

void TestMenu::OnUpdate(float deltaTime)
{
}
void TestMenu::OnRender()
{
}

void TestMenu::OnImGuiRender()
{
	for (auto& test : m_tests)
	{
		if (ImGui::Button(test.first.c_str()))
			m_currentTest = test.second();
	}
}
}