#include <render/entity.hpp>

Entity::Entity(std::shared_ptr<Model> model, const Transform& transform)
    : model(model), transform(transform){}

void Entity::Draw(std::shared_ptr<Shader> shader) const
{
    shader->setMat4("model", transform.ModelMatrix());
    model->Draw();
}


