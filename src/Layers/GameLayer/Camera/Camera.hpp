#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Application/Inputs/Inputs.hpp>

class Camera
{
public:
    Camera(int width, int height) :
        yaw{ -90.0f }, pitch{ 0.0f }, lastX{ 0.0f }, lastY{ 0.0f }, speed{5.0f},
        firstMouse{ true }, sensitivity{ 0.1f }, FOV{ 45.0f }, aspectRatio{(float)width/height}
    {
        position = glm::vec3(0.0f, 2.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);   
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::vec3& GetPosition();

    inline void SetSpeed(float speed) { speed = speed; }
    inline float GetSpeed() { return speed; }
    void FollowPlayer(const glm::vec3& playerPos, const glm::vec3& offset);
    void OnUpdate(float deltaTime);
    void processScrolling(double x, double y);
    void ProcessMouse(float xpos, float ypos);
    void updateAspectRatio(int width, int height);
    void OnEvent(Event& e);

private:
    float aspectRatio;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float speed;
    float yaw;
    float pitch;
    float FOV;

    float lastX;
    float lastY;

    bool firstMouse;
    float sensitivity;
};