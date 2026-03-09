#pragma once

#include <Layers/GameLayer/Scene/Scene.hpp>

class Event;
class SceneManager
{
public:
    SceneManager(const std::vector<Scene>& scenes);

    void OnUpdate(float dt);
    void OnEvent(Event& e);

private:
    std::vector<Scene> m_Scenes;
};
