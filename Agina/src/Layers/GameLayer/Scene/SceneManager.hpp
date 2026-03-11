#pragma once

#include <Layers/GameLayer/Scene/Scene.hpp>

class Event;
class SceneManager
{
public:
    SceneManager(std::vector<std::unique_ptr<Scene>> scenes);

    void OnUpdate(float dt);
    void OnEvent(Event& e);

    Scene& GetCurrentScene() { return *m_Scenes.at(m_CurrentScene); }

private:
    std::vector<std::unique_ptr<Scene>> m_Scenes;
    int m_CurrentScene;
};
