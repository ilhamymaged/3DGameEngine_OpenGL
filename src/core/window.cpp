#include <core/window.hpp>

Window::Window(const std::string &title, int width, int height)
    :width(width), height(height), x(0), y(0)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        throw std::runtime_error("GLFW initialization failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwGetWindowPos(window, &x, &y);
}

Window::~Window()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}


void Window::Close() const
{
    glfwSetWindowShouldClose(window, true);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(window);
}

void Window::PollEvents() const
{
    glfwPollEvents();
}

GLFWwindow* Window::GetGLFWwindow() const
{
    return window;
}

void Window::onEvent(std::shared_ptr<Event> e)
{
    if (e->getType() == EventType::KeyPressed)
    {
        auto keyEvent = static_cast<KeyPressedEvent*>(e.get());
        if (keyEvent->getKey() == GLFW_KEY_ESCAPE) Close();
        //if (keyEvent->getKey() == GLFW_KEY_F11) ToggleFullScreen();
    }
}

void Window::ToggleFullScreen()
{
    if (glfwGetWindowMonitor(window))
    {
        glfwSetWindowMonitor(window, NULL, x, y, width, height, 0);                           
    }
    else 
    {
        glfwGetWindowPos(window, &x, &y);
        glfwGetWindowSize(window, &width, &height);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (!monitor) return; 

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!mode) return; 

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, 
            mode->height, mode->refreshRate);
    }
}