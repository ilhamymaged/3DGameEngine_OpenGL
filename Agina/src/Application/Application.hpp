#pragma once

#include <Application/Renderer/Renderer.hpp>
#include <Application/platform/Window.hpp>
#include <Application/LayerStack/LayerStack.hpp>

class Application
{
public:
    Application(const std::string &title, int width, int height);
    virtual ~Application() = default;

    virtual void Run();
protected:
    virtual void ShutDown();

private:
    Window m_Window;
    LayerStack m_LayerStack;    
};

namespace Agina {
    Application* CreateApplication();
};
