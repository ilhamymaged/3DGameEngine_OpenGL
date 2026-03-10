#include <Application/Platform/Window.hpp>
#include <Application/Inputs/Events.hpp>

Window::Window(const std::string &title, int width, int height)
    :m_Width(width), m_Height(height), m_PosX(0), m_PosY(0)
{
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
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed");
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

void Window::OnEvent(Event& e)
{
    EventDispatcher eventDispatcher(e);
    eventDispatcher.Dispatch<KeyPressed>([&](KeyPressed& e)
    {
        if (e.getKey() == GLFW_KEY_ESCAPE) Close();
    });
}