#include <Core/Time/Time.hpp>    
#include <GLFW/glfw3.h>

namespace Agina
{
    float Time::deltaTime = 0.0f;
    float Time::lastFrame = 0.0f;

    void Time::Update()
    {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
    }

    float Time::GetDeltaTime()
    {
        return deltaTime;
    }
}