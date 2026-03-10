#pragma once

#include <Layers/GameLayer/Scene/Scene.hpp>

class Event;
class SceneManager
{
public:
    SceneManager(const std::vector<Scene>& scenes);

    void OnUpdate(float dt);
    void OnEvent(Event& e);

    Scene& GetCurrentScene() { return m_Scenes.at(m_CurrentScene); }

private:
    std::vector<Scene> m_Scenes;
    int m_CurrentScene;
};
