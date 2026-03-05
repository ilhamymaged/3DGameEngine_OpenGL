#include <scene/player.hpp>

Player::Player(std::shared_ptr<Model> model, const Transform& t, Input& input)
: Entity(model, t), input(input){}

void Player::onEvent(std::shared_ptr<Event> e)
{
    if (e->getType() == EventType::KeyPressed)
    {
        auto key = static_cast<KeyPressedEvent*>(e.get());
        if (key->getKey() == GLFW_KEY_SPACE) std::cout << "Player Jumped!\n";
    }
}

void Player::Update(float deltaTime)
{
    float speed = 5.0f;
    if (input.IsKeyPressed(GLFW_KEY_W))
        transform.position += glm::vec3(0, 0, -1) * speed * deltaTime;
    if (input.IsKeyPressed(GLFW_KEY_S))
        transform.position += glm::vec3(0, 0, 1) * speed * deltaTime;
    if (input.IsKeyPressed(GLFW_KEY_A))
        transform.position += glm::vec3(-1, 0, 0) * speed * deltaTime;
    if (input.IsKeyPressed(GLFW_KEY_D))
        transform.position += glm::vec3(1, 0, 0) * speed * deltaTime;
}
