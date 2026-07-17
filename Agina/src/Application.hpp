#pragma once

#include <Core/Window.hpp>
#include <Core/LayerStack.hpp>

namespace Agina  {   
    class Application
    {
    public:
        Application(const std::string& title, int width, int height);
        virtual ~Application() = default;

        static Application& Get() { return *s_Instance; }

        virtual void Run();
        virtual void ShutDown();
    protected:
        GLFWwindow* GetWindow();
        LayerStack m_LayerStack;
    private:
        static Application* s_Instance;
        bool IsLoggerInit = false;
        Window m_Window;
    };

    Application* CreateApplication();
}
