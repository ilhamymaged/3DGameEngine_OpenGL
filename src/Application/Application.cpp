#include <Application/Application.hpp>
#include <Application/Time/Time.hpp>
#include <Application/Inputs/Inputs.hpp>

Application::Application(const std::string &title, int width, int height)
    : m_Window(title, width, height),
    m_Renderer(m_Window.GetGLFWwindow(),width, height)
{
    Input::Init(m_Window.GetGLFWwindow());
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
            m_LayerStack.OnEvent(*e);

        Time::Update(); 
        m_LayerStack.OnUpdate(Time::GetDeltaTime());

        //m_Renderer.Render();
        m_Window.SwapBuffers();
        Input::Get().ClearEvents();
    }
}