#pragma once

#include <Renderer/Renderer.hpp>
#include <Core/platform/Window.hpp>
#include <Core/LayerStack/LayerStack.hpp>

namespace Agina 
{
    class Application
    {
    public:
        Application(const std::string& title, int width, int height, bool use_ImGui, const std::string& logoPath);
        virtual ~Application() = default;


        virtual void Run();
    protected:
        GLFWwindow* GetWindow();
        virtual void ShutDown();
        
        LayerStack m_LayerStack;
        bool ImGuiUsed;
    private:
        bool m_LoggerInitialized; 
        Window m_Window;
    };

    Application* CreateApplication();
}
