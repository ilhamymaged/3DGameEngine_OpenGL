#pragma once

#include <Application/Renderer/Entity.hpp>
#include <Application/Renderer/SkyBox.hpp>
#include <Application/Renderer/Grid.hpp>
#include <vector>

class Event;
class Scene
{
public:
    Scene(std::vector<Entity> entities);
    Scene(std::vector<Entity> entities, std::unique_ptr<SkyBox> skybox);
    Scene(std::vector<Entity> entities, std::unique_ptr<Grid> grid);
    Scene(std::vector<Entity> entities, std::unique_ptr<SkyBox> skybox,
        std::unique_ptr<Grid> grid);

    Entity& AddEntity(const Entity& e);
    std::vector<Entity>& GetEntities();
    std::function<void(const Entity&)> OnEntityAdded;

    SkyBox* GetSkyBox() { return m_SkyBox.get(); }
    Grid* GetGrid() { return m_Grid.get(); }

    void OnUpdate(float dt);
    void OnEvent(Event& e);
private:
    std::vector<Entity> m_Entities;
    std::unique_ptr<SkyBox> m_SkyBox;
    std::unique_ptr<Grid> m_Grid;
};

