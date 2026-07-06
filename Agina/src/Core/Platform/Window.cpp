#include <Core/Platform/Window.hpp>
#include <Core/Inputs/Events.hpp>
#include <stb_image/stb_image.h>
#include <Core/Logger/Logger.hpp>

namespace Agina
{
    Window::Window(const std::string& title, int width, int height)
        :m_Width(width), m_Height(height), m_PosX(0), m_PosY(0), m_FullSCreen(false)
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

    GLFWwindow* Window::GetGLFWwindow() const
    {
        return m_Window;
    }

    void Window::ToggleFullScreen()
    {
        
        m_FullSCreen = !m_FullSCreen;

        if(m_FullSCreen)
        {

            glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
            glfwGetWindowSize(m_Window, &m_Width, &m_Height);
            
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);
            glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else
        {
            glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowMonitor(m_Window, nullptr, m_PosX, m_PosY, m_Width, m_Height, 0);
        }
    }

    void Window::OnEvent(Event &e)
    {
        EventDispatcher eventDispatcher(e);
        eventDispatcher.Dispatch<WindowResized>([&](WindowResized& wr)
            {
                if (wr.GetNewWidth() == 0 || wr.GetNewHeight() == 0) return;
                glfwSetWindowSize(m_Window, wr.GetNewWidth(), wr.GetNewHeight());
            });
    }
}