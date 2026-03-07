#include <core/application.hpp>
#include <core/time.hpp>

Application::Application(const std::string &title, int width, int height)
    : m_window(title, width, height), m_input(m_window.GetGLFWwindow()),
    m_renderer(m_window.GetGLFWwindow(),width, height, m_input){}

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        m_window.PollEvents();

        for (auto& e : m_input.GetEventQueue())
        {
            m_window.onEvent(e);
            m_renderer.onEvent(e);
        }

        Time::Update(); 
        m_renderer.Update(Time::GetDeltaTime());
        m_input.Update(Time::GetDeltaTime());

        m_renderer.Render();
        m_window.SwapBuffers();

        m_input.ClearEvents();
    }
}