#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/inputs.hpp>

class Camera
{
public:
    Camera(int width, int height, Input& i) :input{ i },
        yaw{ -90.0f }, pitch{ 0.0f }, lastX{ 0.0f }, lastY{ 0.0f },
        firstMouse{ true }, sensitivity{ 0.1f }, FOV{ 45.0f }, aspectRatio{(float)width/height}
    {
        position = glm::vec3(0.0f, 2.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);   
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::mat4 GetViewMatrix() 
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
    }

    void FollowPlayer(const glm::vec3& playerPos, const glm::vec3& offset) 
    {
        position = glm::mix(position, playerPos + offset, 0.1f);
        front = glm::normalize(playerPos - position);
    }

    void Update(float deltaTime)
    {
        float speed = 5.0f;
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

    void processScrolling(double x, double y)
    {
        FOV -= y;
        if (FOV < 45.0f) FOV = 45.0f;
        if (FOV > 90.0f) FOV = 90.0f;
    }

    void ProcessMouse(float xpos, float ypos)
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

    void updateAspectRatio(int width, int height)
    {
        aspectRatio = (float)width / height;
        lastX = (float)width / 2;
        lastY = (float)height / 2;
        //firstMouse = true;  
    }

    void onEvent(std::shared_ptr<Event> e)
    {
        if (input.IsCursorEnabled())
            return;

        if (e->getType() == EventType::MouseMoved)
        {
            auto m = std::static_pointer_cast<MouseMovedEvent>(e);
            ProcessMouse(m->getX(), m->getY());
        }

        if (e->getType() == EventType::MouseScrolled)
        {
            auto m = std::static_pointer_cast<MouseScrolledEvent>(e);
            processScrolling(m->getXoffset(), m->getYoffset());
        }
    }

private:
    float aspectRatio;
    Input& input;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw;
    float pitch;
    float FOV;

    float lastX;
    float lastY;

    bool firstMouse;
    float sensitivity;
};