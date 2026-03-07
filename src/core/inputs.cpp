#include <core/inputs.hpp>
#include <iostream>

Input::Input(GLFWwindow *window) : window(window), cursorEnabled(false)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window,[](GLFWwindow *win, int key, int scancode, int action, int mods)
    {
        Input *input = static_cast<Input *>(glfwGetWindowUserPointer(win));
        if (!input)
            return;

        if (action == GLFW_PRESS)
        {
            input->m_keyStates[key] = true;
            input->eventQueue.push_back(
                std::make_shared<KeyPressedEvent>(key, scancode, mods));
        }
        else if (action == GLFW_RELEASE)
        {
            input->m_keyStates[key] = false;
            input->eventQueue.push_back(
                std::make_shared<KeyReleased>(key, scancode, mods));
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) 
    {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        if (!input) return;

        input->eventQueue.push_back(std::make_shared<MouseScrolledEvent>(xOffset, yOffset));
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) 
    {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        if (!input) return;

        input->eventQueue.push_back(std::make_shared<MouseMovedEvent>(xPos, yPos));
    });

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height)
    {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(win));
        if (!input) return;

        input->eventQueue.push_back(std::make_shared<WindowResized>(width, height));
    });
}

std::vector<std::shared_ptr<Event>> &Input::GetEventQueue()
{
    return eventQueue;
}

void Input::ClearEvents()
{
    eventQueue.clear();
}

bool Input::IsKeyPressed(int key)
{
    auto it = m_keyStates.find(key);
    bool pressed = it != m_keyStates.end() && it->second;
    return pressed;
}

void Input::Update(float deltaTime)
{
    bool ctrlPressed = IsKeyPressed(GLFW_KEY_LEFT_CONTROL);
    if (ctrlPressed && !cursorEnabled)
    {
        cursorEnabled = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (!ctrlPressed && cursorEnabled)
    {
        cursorEnabled = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        eventQueue.push_back(
            std::make_shared<MouseMovedEvent>(x, y)
        );
    }
}

bool Input::IsCursorEnabled()
{
    return cursorEnabled;
}

