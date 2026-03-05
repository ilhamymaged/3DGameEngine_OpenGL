#include <scene/world.hpp>
#include <glm/glm.hpp>

World::World(const std::vector<Entity>& entities): entities(entities) {}

void World::addEntity(const Entity& entity)
{
    entities.push_back(entity);
}

void World::update(float deltaTime)
{
}

const std::vector<Entity>& World::GetEntities() const
{
    return entities;
}


