#include <Application.hpp>
#include <Core/Time/Time.hpp>
#include <Core/Inputs/Inputs.hpp>
#include <ImGui/ImGuiLayer.hpp>

namespace Agina
{
    Application::Application(const std::string &title, int width, int height, bool is)
        : m_Window(title, width, height), m_LayerStack(), ImGuiUsed(is)
    {
        Renderer::Init();
        Input::Init(GetWindow());
        if (ImGuiUsed) ImGuiLayer::Init(GetWindow());
    }

    GLFWwindow* Application::GetWindow()
    {
        return m_Window.GetGLFWwindow();
    }

    void Application::Run()
    {
        m_LayerStack.OnAttach();
        while (!m_Window.ShouldClose())
        {
            m_Window.PollEvents();

            for (auto &e : Input::Get().GetEventQueue())
            {
                m_Window.OnEvent(*e);
                m_LayerStack.OnEvent(*e);
            }

            Time::Update();
            m_LayerStack.OnUpdate(Time::GetDeltaTime());

            Renderer::Clear();
            m_LayerStack.OnRender();

            if (ImGuiUsed)
            {
                ImGuiLayer::BeginFrame();
                m_LayerStack.OnImGuiRender();
                ImGuiLayer::EndFrame();
            }
            m_Window.SwapBuffers();
            Input::Get().ClearEvents();
        }

        if (ImGuiUsed) ImGuiLayer::ShutDown();
    }

    void Application::ShutDown()
    {
        m_LayerStack.OnDetach();
    }
}