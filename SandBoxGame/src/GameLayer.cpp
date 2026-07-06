#include "GameLayer.hpp"
#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Transform.hpp>
#include <Application.hpp>
#include <Core/Inputs/KeyMappings.hpp>
#include <Renderer/Camera.hpp>
#include <Core/Inputs/Inputs.hpp>

using namespace Agina;

std::shared_ptr<Material> mat = nullptr;
std::shared_ptr<Material> playerMat = nullptr;
std::shared_ptr<Mesh> mesh = nullptr;
Transform triangleTransform;
Transform player;
Camera camera;

void GameLayer::OnAttach()
{
	camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f));

	mat = Material::Create(MaterialType::UNLIT);
	playerMat = Material::Create(MaterialType::UNLIT);

	mesh = Mesh::Create(MeshType::TRIANGLE);
	
	triangleTransform = Transform(glm::vec3(2.0f, 0.0f, 0.0));
	player = Transform();

	mat->Set("uColor", glm::vec3(0.0f, 1.0f, 0.0f));
	playerMat->Set("uColor", glm::vec3(1.0f, 0.0f, 0.0f));

	camera.SetMode(CameraMode::Follow);
	camera.SetFocusTarget(&player.Position);
	camera.SetFollowOffset(glm::vec3(0.0f, 1.5f, 5.0f));
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<KeyPressed>([&](KeyPressed& key) 
	{
		if (key.getKey() == static_cast<int>(Key::Escape))
		{
			Application::Get().ShutDown();
		}
	});

	camera.OnEvent(e);
}

void GameLayer::OnUpdate(float dt)
{
	camera.Update(dt);

	auto& input = Input::Get();
	const float playerSpeed = 3.0f;

	// These axes are world-aligned; replace with player-local movement if you add orientation
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	if (input.IsKeyPressed(static_cast<int>(Key::W))) player.Position += forward * playerSpeed * dt;
	if (input.IsKeyPressed(static_cast<int>(Key::S))) player.Position -= forward * playerSpeed * dt;
	if (input.IsKeyPressed(static_cast<int>(Key::A))) player.Position -= right * playerSpeed * dt;
	if (input.IsKeyPressed(static_cast<int>(Key::D))) player.Position += right * playerSpeed * dt;
	if (input.IsKeyPressed(static_cast<int>(Key::Space))) player.Position.y += playerSpeed * dt;
	if (input.IsKeyPressed(static_cast<int>(Key::LeftShift))) player.Position.y -= playerSpeed * dt;
}

void GameLayer::OnRender()
{
	Renderer::BeginScene(camera);
	Renderer::Draw(*mesh, *mat, triangleTransform);
	Renderer::Draw(*mesh, *playerMat, player);
	Renderer::EndScene();
}

void GameLayer::OnUIRender()
{

}
