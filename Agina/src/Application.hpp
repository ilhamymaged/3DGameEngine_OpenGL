#pragma once

#include <Renderer/Renderer.hpp>
#include <Core/Platform/Window.hpp>
#include <Core/LayerStack/LayerStack.hpp>

namespace Agina 
{
    class Application
    {
    public:
        Application(const std::string& title, int width, int height, bool use_ImGui);
        virtual ~Application() = default;


        virtual void Run();
        virtual void ShutDown();
    protected:
        GLFWwindow* GetWindow();
        
        LayerStack m_LayerStack;
        bool ImGuiUsed;
    private:
        bool IsLoggerInit = false;
        Window m_Window;
    };

    Application* CreateApplication();
}
