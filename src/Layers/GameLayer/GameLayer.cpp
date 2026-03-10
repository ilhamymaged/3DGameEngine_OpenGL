#include <Layers/GameLayer/GameLayer.hpp>
#include <Application/Renderer/Renderer.hpp>
#include <Application/Renderer/ShaderLibrary.hpp>

GameLayer::GameLayer(int width, int height)
{
	m_Camera = std::make_unique<Camera>(width, height);
	
	std::vector<Scene> scenes;
	std::vector<Entity> entities;

	std::shared_ptr<Model> model = 
		std::make_shared<Model>("../../../assets/models/backpack/backpack.obj");
	Transform transform;

	entities.push_back(Entity(model, transform));
	Scene scene(entities);

	scenes.push_back(scene);
	m_SceneManager = std::make_unique<SceneManager>(scenes);
}

void GameLayer::OnAttach(){}
void GameLayer::OnDetach(){}

void GameLayer::OnUpdate(float dt)
{
	m_SceneManager->OnUpdate(dt);
	m_Camera->OnUpdate(dt);
}

void GameLayer::OnEvent(Event& e)
{
	m_SceneManager->OnEvent(e);
	m_Camera->OnEvent(e);
}

void GameLayer::OnRender(Renderer& renderer)
{
	Scene& scene = m_SceneManager->GetCurrentScene();
	renderer.RenderScene(scene, *m_Camera);
}
