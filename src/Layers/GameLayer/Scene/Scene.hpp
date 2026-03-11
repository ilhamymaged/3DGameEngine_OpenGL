#pragma once

#include <entt/entt.hpp>

class Event;
class Scene
{
public:
    Scene(entt::registry& e);

    inline entt::registry& GetRegistry() { return m_Registry; }

    void OnUpdate(float dt);
    void OnEvent(Event& e);
private:
    entt::registry m_Registry;
};

