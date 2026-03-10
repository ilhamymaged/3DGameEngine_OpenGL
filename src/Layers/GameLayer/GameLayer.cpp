#include <Layers/GameLayer/GameLayer.hpp>
#include <Application/Renderer/Renderer.hpp>
#include <Application/Renderer/ShaderLibrary.hpp>
#include <Application/Renderer/SkyBox.hpp>

GameLayer::GameLayer(int width, int height)
{
	std::vector<std::string> faces =
	{
		"../../../assets/skyboxes/sky1/right.jpg",
		"../../../assets/skyboxes/sky1/left.jpg",
		"../../../assets/skyboxes/sky1/top.jpg",
		"../../../assets/skyboxes/sky1/bottom.jpg",
		"../../../assets/skyboxes/sky1/front.jpg",
		"../../../assets/skyboxes/sky1/back.jpg"
	};

	auto skyBox = std::make_unique<SkyBox>(faces);
	m_Camera = std::make_unique<Camera>(width, height);
	
	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<Entity> entities;

	std::shared_ptr<Model> model = 
		std::make_shared<Model>("../../../assets/models/backpack/backpack.obj");
	Transform transform;

	entities.emplace_back(Entity(model, transform));
	scenes.emplace_back(std::make_unique<Scene>(entities, std::move(skyBox)));
	m_SceneManager = std::make_unique<SceneManager>(std::move(scenes));
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

void GameLayer::OnRender()
{
	Renderer::Clear({ 0.1f, 0.1f, 0.15f, 1.0f });
	Scene& scene = m_SceneManager->GetCurrentScene();
	SceneRenderer::Render(scene, *m_Camera);
	
	auto skyBox = scene.GetSkyBox();
	if(skyBox) skyBox->Draw(m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix());
}
