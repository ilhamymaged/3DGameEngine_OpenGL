#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

class Window 
{
public:
    Window(const std::string &title, int width, int height);
    ~Window();
    
    bool ShouldClose() const;
    void SwapBuffers() const;
    void PollEvents() const;
    void Close() const;

    GLFWwindow *GetGLFWwindow() const;
private:
    GLFWwindow *m_Window;
    int m_Width;
    int m_Height;
    int m_PosX;
    int m_PosY;
};

