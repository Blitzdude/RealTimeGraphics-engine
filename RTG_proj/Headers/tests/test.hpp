#pragma once
#include <functional> // for std::function
#include <vector>
#include "GLFW/glfw3.h"

namespace test {

class Test 
{
   public:
	Test() {}
	virtual ~Test() {}
    
	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
	virtual void KeyboardInput(GLFWwindow* window) {}
	virtual void MouseInput(double xpos, double ypos) {}

};
class TestMenu : public Test
{
public:
	TestMenu(Test*& currenTestPtr);
	~TestMenu();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;
	//void MouseInput(double xpos, double ypos) ;

	template <typename T>
	void RegisterTest(const std::string& name)
	{
		std::cout << "Registering Test " << name << std::endl;
		m_tests.push_back(std::make_pair(name, []() {return new T(); }));
	}

private:

	Test*& m_currentTest;
	std::vector<std::pair<std::string, std::function<Test*()>>> m_tests; 
	// we have vector of tests, with string - function pair, because we don't want to create tests yet.
};
}