#include "Camera.hpp"

namespace Agina
{
    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
    }

    glm::vec3& Camera::GetPosition()
    {
        return position;
    }

    void Camera::FollowPlayer(const glm::vec3& playerPos, const glm::vec3& offset)
    {
        position = glm::mix(position, playerPos + offset, 0.1f);
        front = glm::normalize(playerPos - position);
    }

    void Camera::OnUpdate(float deltaTime)
    {
        Input& input = Input::Get();
        glm::vec3 right = glm::normalize(glm::cross(front, up));

        if (input.IsKeyPressed(GLFW_KEY_W))
            position += front * speed * deltaTime;
        if (input.IsKeyPressed(GLFW_KEY_S))
            position -= front * speed * deltaTime;
        if (input.IsKeyPressed(GLFW_KEY_A))
            position -= right * speed * deltaTime;
        if (input.IsKeyPressed(GLFW_KEY_D))
            position += right * speed * deltaTime;
        if (input.IsKeyPressed(GLFW_KEY_SPACE))
            position += up * speed * deltaTime;
    }

    void Camera::processScrolling(double x, double y)
    {
        FOV -= y;
        if (FOV < 45.0f) FOV = 45.0f;
        if (FOV > 90.0f) FOV = 90.0f;
    }

    void Camera::ProcessMouse(float xpos, float ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;

        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(direction);
    }

    void Camera::updateAspectRatio(int width, int height)
    {
        aspectRatio = (float)width / height;
        lastX = (float)width / 2;
        lastY = (float)height / 2;
        //firstMouse = true;  
    }

    void Camera::OnEvent(Event& e)
    {
        Input& input = Input::Get();
        if (input.IsCursorEnabled())
            return;

        EventDispatcher eventDispatcher(e);
        eventDispatcher.Dispatch<MouseMoved>([&](MouseMoved& event)
            {
                ProcessMouse(event.getX(), event.getY());
            });

        eventDispatcher.Dispatch<MouseScrolled>([&](MouseScrolled& event)
            {
                processScrolling(event.getXoffset(), event.getYoffset());
            });
    }
}