#pragma once

#include <Application/Renderer/Renderer.hpp>
#include <Application/platform/Window.hpp>
#include <Application/LayerStack/LayerStack.hpp>

class Application
{
public:
    Application(const std::string &title, int width, int height);
    ~Application();

    void Run();
private:
    Window m_Window;
    Renderer m_Renderer;

    LayerStack m_LayerStack;    
};