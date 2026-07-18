#include <Core/Window.hpp>
#include <Core/Events.hpp>
#include <stb_image/stb_image.h>
#include <Core/Logger.hpp>
#include <Core/KeyMappings.hpp>
#include <Core/Inputs.hpp>

namespace Agina
{
    Window::Window(const std::string& title, int width, int height)
        :m_Width(width), m_Height(height), m_PosX(0), m_PosY(0), m_FullScreen(false)
    {

        glfwSetErrorCallback([](int error, const char* description) 
        {
			AG_CORE_ERROR("GLFW Error {}: {}", error, description);
        });

        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            throw std::runtime_error("GLFW initialization failed");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            AG_CORE_ERROR("Failed To Create GLFW Window");
            glfwTerminate();
            throw std::runtime_error("GLFW Window Creation Failed");
        }

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(0);
        glfwSetWindowPos(m_Window, 100, 70);
        glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
    }

    Window::~Window()
    {
        if (m_Window) glfwDestroyWindow(m_Window);
        glfwTerminate();
    }


    void Window::Close() const
    {
        glfwSetWindowShouldClose(m_Window, true);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(m_Window);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    void Window::SetWindowTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    GLFWwindow* Window::GetGLFWwindow() const
    {
        return m_Window;
    }

    void Window::OnEvent(Event &e)
    {
        EventDispatcher eventDispatcher(e);
        eventDispatcher.Dispatch<WindowResized>([&](WindowResized& wr)
            {
                if (wr.GetNewWidth() == 0 || wr.GetNewHeight() == 0) return;
                m_Width = wr.GetNewWidth();
                m_Height = wr.GetNewHeight();
            });

        eventDispatcher.Dispatch<KeyPressed>([&](KeyPressed& kp)
            {
                if (kp.getKey() == static_cast<int>(Key::LeftControl))
                {
                    if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
                    {
                        Input::Get().UnLockMouse();
                    }
                    else 
                    {
                        Input::Get().LockMouse();
                    }
                }

                if (kp.getKey() == static_cast<int>(Key::F11))
                {
                    ToggleFullScreen();
                }

            });
    }

    void Window::ToggleFullScreen()
    {
        m_FullScreen = !m_FullScreen;

        if (m_FullScreen)
        {
            glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
            glfwGetWindowSize(m_Window, &m_Width, &m_Height);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if (monitor)
            {
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                if (mode)
                {
                    glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);
                    glfwSetWindowPos(m_Window, 0, 0);
                    glfwSetWindowSize(m_Window, mode->width, mode->height);
                }
            }
        }
        else
        {
            glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowSize(m_Window, m_Width, m_Height);
            glfwSetWindowPos(m_Window, m_PosX, m_PosY);
        }

    }

}