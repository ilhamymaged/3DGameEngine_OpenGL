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
	auto grid = std::make_unique<Grid>();

	m_Camera = std::make_unique<Camera>(width, height);
	
	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<Entity> entities;

	std::shared_ptr<Model> model =
		std::make_shared<Model>("../../../assets/models/backpack/backpack.obj");
	Transform transform;
	transform.position = glm::vec3(0.0f, 2.0f, 0.0f);

	entities.emplace_back(Entity(model, transform));
	
	auto scene = std::make_unique<Scene>(entities/*, std::move(skyBox)*/, std::move(grid));
	scenes.emplace_back(std::move(scene));
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

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResized>([&](WindowResized& w)
		{
			Renderer::SetViewport(0, 0, w.GetNewWidth(), w.GetNewHeight());
			m_Camera->updateAspectRatio(w.GetNewWidth(), w.GetNewHeight());
		});
}

void GameLayer::OnRender()
{
	Renderer::Clear({ 0.1f, 0.1f, 0.15f, 1.0f });
	Scene& scene = m_SceneManager->GetCurrentScene();

	auto grid = scene.GetGrid();
	if (grid) grid->Draw(m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), 
		m_Camera->GetPosition());

	SceneRenderer::Render(scene, *m_Camera);

	auto skyBox = scene.GetSkyBox();
	if(skyBox) skyBox->Draw(m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix());
}
