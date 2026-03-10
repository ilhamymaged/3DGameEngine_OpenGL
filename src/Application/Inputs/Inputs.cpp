#include <Application/Inputs/Inputs.hpp>
#include <iostream>

Input* Input::m_Instance = nullptr;

Input::Input(GLFWwindow *window) : m_Window(window), m_CursorEnabled(false)
{
    glfwSetWindowUserPointer(window, this);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window,[](GLFWwindow *win, int key, int scancode, int action, int mods)
    {
        Input *input = static_cast<Input *>(glfwGetWindowUserPointer(win));
        if (!input)
            return;

        if (action == GLFW_PRESS)
        {
            input->m_KeyStates[key] = true;
            input->m_EventQueue.push_back(std::make_unique<KeyPressed>(key, scancode, mods));
        }
        else if (action == GLFW_RELEASE)
        {
            input->m_KeyStates[key] = false;
            input->m_EventQueue.push_back(std::make_unique<KeyReleased>(key, scancode, mods));
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) 
    {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        if (!input) return;

        input->m_EventQueue.push_back(std::make_unique<MouseScrolled>(xOffset, yOffset));
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) 
    {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        if (!input) return;

        input->m_EventQueue.push_back(std::make_unique<MouseMoved>(xPos, yPos));
    });

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height)
    {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
        if (!input) return;

        input->m_EventQueue.push_back(std::make_unique<WindowResized>(width, height));
    });
}

std::vector<std::unique_ptr<Event>>& Input::GetEventQueue()
{
    return m_EventQueue;
}

void Input::ClearEvents()
{
    m_EventQueue.clear();
}

void Input::Init(GLFWwindow* window)
{
    m_Instance = new Input(window);
}

bool Input::IsKeyPressed(int key)
{
    auto it = m_KeyStates.find(key);
    bool pressed = it != m_KeyStates.end() && it->second;
    return pressed;
}

bool Input::IsCursorEnabled()
{
    return m_CursorEnabled;
}

Input& Input::Get()
{
    return *m_Instance;
}