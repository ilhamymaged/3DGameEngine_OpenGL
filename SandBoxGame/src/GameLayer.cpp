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
std::shared_ptr<Material> terrainMat = nullptr;

std::shared_ptr<Mesh> mesh = nullptr;
std::shared_ptr<Mesh> sphereMesh = nullptr;
std::shared_ptr<Mesh> terrainMesh = nullptr;

Transform triangleTransform;
Transform player;
Transform terrain;

Camera camera;

void GameLayer::OnAttach()
{	
	camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f));

	mat = Material::Create(MaterialType::LIT);
	playerMat = Material::Create(MaterialType::LIT);
	terrainMat = Material::Create(MaterialType::LIT);

	mesh = Mesh::Create(MeshType::TRIANGLE);
	sphereMesh = Mesh::Create(MeshType::SPHERE);
	terrainMesh = Mesh::Create(MeshType::TERRAIN);
	
	triangleTransform = Transform(glm::vec3(2.0f, 1.0f, 0.0));
	player = Transform(glm::vec3(0.0f, 2.0f, 0.0f));
	terrain = Transform();

	mat->Set("u_Color", glm::vec3(0.0f, 1.0f, 0.0f));
	mat->Set("u_HasColor", true);
	playerMat->Set("u_Color", glm::vec3(1.0f, 0.0f, 0.0f));
	playerMat->Set("u_HasColor", true);
	terrainMat->Set("u_Color", glm::vec3(0.3f, 0.2f, 0.1f));
	terrainMat->Set("u_HasColor", true);

	//camera.SetMode(CameraMode::Follow);
	//camera.SetFocusTarget(&player.Position);
	//camera.SetFollowOffset(glm::vec3(0.0f, 1.5f, 5.0f));
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

	//auto& input = Input::Get();
	//const float playerSpeed = 3.0f;

	//auto forward = glm::vec3(0.0f, 0.0f, -1.0f);
	//auto right = glm::vec3(1.0f, 0.0f, 0.0f);

	//if (input.IsKeyPressed(static_cast<int>(Key::W))) player.Position += forward * playerSpeed * dt;
	//if (input.IsKeyPressed(static_cast<int>(Key::S))) player.Position -= forward * playerSpeed * dt;
	//if (input.IsKeyPressed(static_cast<int>(Key::A))) player.Position -= right * playerSpeed * dt;
	//if (input.IsKeyPressed(static_cast<int>(Key::D))) player.Position += right * playerSpeed * dt;
	//if (input.IsKeyPressed(static_cast<int>(Key::Space))) player.Position.y += playerSpeed * dt;
	//if (input.IsKeyPressed(static_cast<int>(Key::LeftShift))) player.Position.y -= playerSpeed * dt;
}

void GameLayer::OnRender()
{
	glm::vec3 directionalLightPosition = glm::vec3(-10.0f, 20.0f, -5.0f);
	glm::vec3 lightTargetPoint = glm::vec3(0.0f, 0.0f, 0.0f);

	Renderer::BeginShadowPass(directionalLightPosition, lightTargetPoint);
	Renderer::Draw(*mesh, *mat, triangleTransform);
	Renderer::Draw(*sphereMesh, *playerMat, player);
	Renderer::Draw(*terrainMesh, *terrainMat, terrain);
	Renderer::EndShadowPass();

	Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	Renderer::BeginScene(camera);
	Renderer::Draw(*mesh, *mat, triangleTransform);
	Renderer::Draw(*sphereMesh, *playerMat, player);
	Renderer::Draw(*terrainMesh, *terrainMat, terrain);
	Renderer::EndScene();
}


void GameLayer::OnDetach()
{

}

void GameLayer::OnUIRender()
{

}
