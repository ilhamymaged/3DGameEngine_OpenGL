#pragma once

#include <render/renderer.hpp>
#include <core/window.hpp>
#include <core/inputs.hpp>

class Application
{
public:
    Application(const std::string &title, int width, int height);
    ~Application() = default;

    void Run();
private:
    Window m_window;
    Input m_input;
    Renderer m_renderer;
};