#include <Application/Application.hpp>
#include <Application/Time/Time.hpp>
#include <Application/Inputs/Inputs.hpp>
#include <Layers/GameLayer/GameLayer.hpp>

Application::Application(const std::string &title, int width, int height)
    : m_Window(title, width, height), m_LayerStack()
{
    Renderer::Init();
    Input::Init(m_Window.GetGLFWwindow());

    m_LayerStack.PushLayer(new GameLayer(width, height));
    m_LayerStack.OnAttach();
}

Application::~Application()
{
    m_LayerStack.OnDetach();
}

void Application::Run()
{
    while (!m_Window.ShouldClose())
    {
        m_Window.PollEvents();

        for (auto& e : Input::Get().GetEventQueue())
        {
            m_Window.OnEvent(*e);
            m_LayerStack.OnEvent(*e);
        }

        Time::Update(); 
        m_LayerStack.OnUpdate(Time::GetDeltaTime());

        m_LayerStack.OnRender();

        m_Window.SwapBuffers();
        Input::Get().ClearEvents();
    }
}