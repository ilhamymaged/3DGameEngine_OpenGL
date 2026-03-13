#pragma once

#include <iostream> 
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <Core/Inputs/Events.hpp>
#include <vector>
#include <unordered_map>

namespace Agina
{
    class Input
    {
    public:
        static void Init(GLFWwindow* window);
        static Input& Get();
        
        void LockMouse();
        void UnLockMouse();

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
}
