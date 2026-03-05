#pragma once

#include <scene/world.hpp>
#include <render/shader.hpp>
#include <core/layer.hpp>
#include <scene/player.hpp>
#include <scene/camera.hpp>
#include <core/inputs.hpp>

class SceneManager : public Layer
{
public:
    SceneManager(int width, int height, Input& i);
    void setCurrentScene(std::unique_ptr<World> scene);
    std::unique_ptr<World> GetCurrentScene() const;

    void Update(float deltaTime);
    void onEvent(std::shared_ptr<Event> e);

    inline Camera& GetCamera() { return *camera; }
    //inline Player& GetPlayer() { return *player; }
    inline World& GetWorld() { return *currentScene; }

private:
    std::unique_ptr<World> currentScene;
    //std::unique_ptr<Player> player;
    std::unique_ptr<Camera> camera;
};
