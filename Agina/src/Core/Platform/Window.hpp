#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

namespace Agina
{
    class Event;
    class Window
    {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        bool ShouldClose() const;
        void SwapBuffers() const;
        void PollEvents() const;

        GLFWwindow* GetGLFWwindow() const;
        void OnEvent(Event& e);

    private:
        void Close() const;
        void ToggleFullScreen();

    private:
        GLFWwindow* m_Window;
        bool m_FullSCreen;
        int m_Width;
        int m_Height;
        int m_PosX;
        int m_PosY;
    };
}
