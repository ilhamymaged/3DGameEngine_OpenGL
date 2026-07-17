#include <Application.hpp>
#include <Core/Time.hpp>
#include <Core/Inputs.hpp>
#include <Core/Logger.hpp>
#include <Core/AssetManager.hpp>
#include <UI/UI.hpp>
#include <Sound/AudioSystem.hpp>
#include <Renderer/Renderer.hpp>
#include <Core/FileSystem.hpp>

namespace Agina
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string &title, int width, int height)
        :   IsLoggerInit((Logger::InitEngineLogger(), true)),
            m_Window(title, width, height),
            m_LayerStack()
    {
        s_Instance = this;

        FileSystem::Init("");
        Input::Init(GetWindow());
        UI::Init(GetWindow());
        AudioSystem::Init();
        Renderer::Init(width, height);
        Logger::InitClientLogger(title);

        AG_CORE_INFO("Engine Initialized Perfectly!");
    }

    GLFWwindow* Application::GetWindow()
    {
        return m_Window.GetGLFWwindow();
    }

    void Application::Run()
    {
        m_LayerStack.OnAttach();

        float fpsTimer = 0.0f;
        int frameCount = 0;

        while (!m_Window.ShouldClose())
        {
            Time::Update();
            float dt = Time::GetDeltaTime();
            fpsTimer += dt;
            frameCount++;

            if (fpsTimer >= 1.0f)
            {
                int fps = frameCount; 
                std::string title = "FPS: " + std::to_string(fps);
                m_Window.SetWindowTitle(title);

                fpsTimer = 0.0f;
                frameCount = 0;
            }

            m_Window.PollEvents();

            for (auto &e : Input::Get().GetEventQueue())
            {
                m_Window.OnEvent(*e);
                Renderer::OnEvent(*e);
                m_LayerStack.OnEvent(*e);
            }

            m_LayerStack.OnUpdate(dt);

            Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            m_LayerStack.OnRender();

            UI::BeginFrame();
            m_LayerStack.OnUIRender();
            UI::EndFrame();

            m_Window.SwapBuffers();
            Input::Get().ClearEvents();
        }

        AssetManager::Clear();
        UI::ShutDown();
        AudioSystem::ShutDown();
        m_LayerStack.OnDetach();
        Renderer::Shutdown();
    }

    void Application::ShutDown()
    {
        m_Window.Close();
    }
}
