#pragma once

#include <render/entity.hpp>
#include <render/shader.hpp>
#include <vector>

class World
{
public:
    World(const std::vector<Entity>& entities);
    void addEntity(const Entity& entity);
    void update(float deltaTime);

    const std::vector<Entity>& GetEntities() const;

private:
    std::vector<Entity> entities;
};

