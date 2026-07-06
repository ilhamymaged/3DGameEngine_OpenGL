#include <Application.hpp>
#include <Core/Time/Time.hpp>
#include <Core/Inputs/Inputs.hpp>
#include <Core/Logger/Logger.hpp>
#include <Core/AssetManager/AssetManager.hpp>
#include <UI/UI.hpp>
#include <Sound/AudioSystem.hpp>
#include <Renderer/Renderer.hpp>
#include <Core/FileSystem/FileSystem.hpp>

namespace Agina
{
    Application::Application(const std::string &title, int width, int height, bool is)
        :   IsLoggerInit((Logger::InitEngineLogger(), true)),
            m_Window(title, width, height),
            m_LayerStack()
    {
        auto path = std::filesystem::current_path()
            .parent_path()
            .parent_path()
            .parent_path()
            .parent_path();
        FileSystem::Init(path);
        Input::Init(GetWindow());
        UI::Init(GetWindow());
        AudioSystem::Init();
        Renderer::Init();
        Logger::InitClientLogger(title);

        AG_CORE_INFO("Engine Initialized");
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

            Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            m_LayerStack.OnRender();

            UI::BeginFrame();
            m_LayerStack.OnUIRender();
            UI::EndFrame();

            Time::Update();
            m_LayerStack.OnUpdate(Time::GetDeltaTime());
            m_Window.SwapBuffers();
            
            Input::Get().ClearEvents();
        }
    }

    void Application::ShutDown()
    {
        AssetManager::Clear();
        UI::ShutDown();
        AudioSystem::ShutDown();
        m_LayerStack.OnDetach();
    }
}
