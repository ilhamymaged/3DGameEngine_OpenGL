#include <Application/Renderer/Entity.hpp>

Entity::Entity(std::shared_ptr<Model> model, const Transform& transform)
    : m_Model(model), m_Transform(transform){}



