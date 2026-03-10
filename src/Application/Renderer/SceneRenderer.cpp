#include <Application/Renderer/SceneRenderer.hpp>
#include <Application/Renderer/Renderer.hpp>
#include <Layers/GameLayer/Scene/Scene.hpp>
#include <Application/Renderer/Entity.hpp>
#include <Layers/GameLayer/Camera/Camera.hpp>

SceneData* SceneRenderer::s_SceneData = nullptr;

void SceneRenderer::Render(Scene& scene, Camera& camera)
{
	SceneData data;
    data.ViewMatrix = camera.GetViewMatrix();
    data.ProjectionMatrix = camera.GetProjectionMatrix();
    data.cameraPosition = camera.GetPosition();

    s_SceneData = &data;

    std::vector<Entity>& entities = scene.GetEntities();
    for (auto& entity : entities)
        RenderEntity(entity);

    s_SceneData = nullptr; 
}

void SceneRenderer::RenderEntity(Entity& entity)
{
    glm::mat4 modelMatrix = entity.GetModelMatrix();
    Model& model = entity.GetModel();
    model.Draw(*s_SceneData, modelMatrix);
}
