#pragma once

namespace test {

class Test 
{
   public:
    Test() {}
    virtual ~Test() {}
    
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
	virtual void MouseInput(double xpos, double ypos) {}

};
	
}