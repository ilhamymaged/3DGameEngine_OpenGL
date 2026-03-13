#pragma once

#include <Renderer/Renderer.hpp>
#include <Core/platform/Window.hpp>
#include <Core/LayerStack/LayerStack.hpp>

namespace Agina 
{
    class Application
    {
    public:
        Application(const std::string& title, int width, int height, bool use_ImGui);
        virtual ~Application() = default;


        virtual void Run();
    protected:
        GLFWwindow* GetWindow();
        virtual void ShutDown();
        
        LayerStack m_LayerStack;
        bool ImGuiUsed;
    private:
        Window m_Window;
    };

    Application* CreateApplication();
}
