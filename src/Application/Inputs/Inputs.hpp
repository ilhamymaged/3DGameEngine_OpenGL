#pragma once

#include <iostream> 
#include <GLFW/glfw3.h>
#include <Application/Inputs/Events.hpp>
#include <vector>
#include <unordered_map>

class Input
{
public:
    static void Init(GLFWwindow* window);
    static Input& Get();

    bool IsKeyPressed(int key);

    void ClearEvents();
    std::vector<std::unique_ptr<Event>>& GetEventQueue();

    void Update(float deltaTime);
    bool IsCursorEnabled();

private:
    Input(GLFWwindow* window);

private:
    static Input* m_Instance;

    GLFWwindow* m_Window;
    std::unordered_map<int, bool> m_KeyStates;
    std::vector<std::unique_ptr<Event>> m_EventQueue;
    bool m_CursorEnabled;
};

