#pragma once

#include <Application/Renderer/Entity.hpp>
#include <vector>

class Event;
class Scene
{
public:
    Scene(std::vector<Entity> entities);
    std::vector<Entity>& GetEntities();

    void OnUpdate(float dt);
    void OnEvent(Event& e);
private:
    std::vector<Entity> m_Entities;
};

