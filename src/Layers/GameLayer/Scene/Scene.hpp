#pragma once

#include <Application/Renderer/Entity.hpp>
#include <Application/Renderer/SkyBox.hpp>
#include <vector>

class Event;
class Scene
{
public:
    Scene(std::vector<Entity> entities);
    Scene(std::vector<Entity> entities, std::unique_ptr<SkyBox> skybox);
    std::vector<Entity>& GetEntities();

    SkyBox* GetSkyBox() { return m_SkyBox.get(); }

    void OnUpdate(float dt);
    void OnEvent(Event& e);
private:
    std::vector<Entity> m_Entities;
    std::unique_ptr<SkyBox> m_SkyBox;
};

