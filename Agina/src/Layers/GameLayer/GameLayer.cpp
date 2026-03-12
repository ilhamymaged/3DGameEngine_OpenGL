#include <Application/Renderer/Renderer.hpp>
#include <Layers/GameLayer/GameLayer.hpp>
#include <Layers/GameLayer/ECS/Components.hpp>
#include <Layers/GameLayer/Systems/Systems.hpp>

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

	entt::registry registry;

	//auto skyBox = registry.create();
	//registry.emplace<SkyBoxComponent>(skyBox, faces);

	auto grid = registry.create();
	registry.emplace<GridComponent>(grid);

	auto camera = registry.create();
	registry.emplace<CameraComponent>(camera, Camera(width, height));

	std::vector<std::unique_ptr<Scene>> scenes;

	Model model("backpack/backpack.obj");
	Transform transform;
	transform.position = glm::vec3(0.0f, 2.0f, 0.0f);

	auto backPack = registry.create();
	registry.emplace<MeshRenderer>(backPack, model);
	registry.emplace<Transform>(backPack, transform);

	auto scene = std::make_unique<Scene>(registry);
	scenes.emplace_back(std::move(scene));
	m_SceneManager = std::make_unique<SceneManager>(std::move(scenes));
}

void GameLayer::OnAttach(){}
void GameLayer::OnDetach(){}

void GameLayer::OnUpdate(float dt)
{
	m_SceneManager->OnUpdate(dt);	

	entt::registry& registry = m_SceneManager->GetCurrentScene().GetRegistry();
	CameraSystem::OnUpdate(registry, dt);
}

void GameLayer::OnEvent(Event& e)
{	
	m_SceneManager->OnEvent(e);

	entt::registry& registry = m_SceneManager->GetCurrentScene().GetRegistry();
	CameraSystem::onEvent(registry, e);
}

void GameLayer::OnRender()
{
	Renderer::Clear({ 0.1f, 0.1f, 0.15f, 1.0f });
	
	entt::registry& registry = m_SceneManager->GetCurrentScene().GetRegistry();
	RenderSystem::OnRender(registry);
}
