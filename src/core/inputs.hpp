#pragma once

#include <iostream> 
#include <GLFW/glfw3.h>
#include <event/events.hpp>
#include <vector>
#include <unordered_map>

class Input
{
public:
    Input(GLFWwindow* window);
    bool IsKeyPressed(int key);

    void ClearEvents();
    std::vector<std::shared_ptr<Event>>& GetEventQueue();

    void Update(float deltaTime);

    bool IsCursorEnabled();
private:
    GLFWwindow* window;
    std::unordered_map<int, bool> m_keyStates;
    std::vector<std::shared_ptr<Event>> eventQueue;
    bool cursorEnabled;
};

