#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <core/layer.hpp>

class Window : public Layer
{
public:
    Window(const std::string &title, int width, int height);
    ~Window();
    
    bool ShouldClose() const;
    void SwapBuffers() const;
    void PollEvents() const;

    void Close() const;

    GLFWwindow *GetGLFWwindow() const;

    void onEvent(std::shared_ptr<Event> e) override;
    void ToggleFullScreen();

private:
    GLFWwindow *window;
    int width;
    int height;
    int x;
    int y;
};
