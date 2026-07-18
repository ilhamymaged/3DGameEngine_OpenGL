#include "GameLayer.hpp"

#include <ECS/PhysicsSystem.hpp>
#include <ECS/RenderSystem.hpp>
#include <ECS/Components.hpp>

#include <Application.hpp>
#include <Core/KeyMappings.hpp>

#include <Core/AssetManager.hpp>
#include <Renderer/Material.hpp>

#include <Core/MathTypes.hpp>

#include <UI/UI.hpp>

#include <Core/Logger.hpp>

#include <Core/FileSystem.hpp>

using namespace Agina;
void GameLayer::OnAttach()
{	
	const std::string assetPath = (FileSystem::AppAssets()).string();

	auto camera = m_Scene.CreateEntity();
	camera.AddComponent<CameraComponent>();
	camera.AddComponent<TagComponent>("Camera");

	auto sphere = m_Scene.CreateEntity();
	sphere.AddComponent<TagComponent>("Sphere");
	sphere.AddComponent<Transform>(Vec3(5.0, 4.0f, 0.0));
	auto& sMat = sphere.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::SPHERE), 
		Material::Create(MaterialType::LIT));
	sMat.MaterialAsset->Set("u_HasColor", true);
	sMat.MaterialAsset->Set("u_Color", Vec3(0.118f, 0.69f, 0.655f));
	sMat.MaterialAsset->Set("u_HasAlbedoTexture", false);
	sMat.MaterialAsset->Set("u_HasNormalTexture", false);
	sMat.MaterialAsset->Set("u_HasSpecularTexture", false);

	auto cube = m_Scene.CreateEntity();
	cube.AddComponent<TagComponent>("Cube");
	cube.AddComponent<Transform>(Vec3(2.0, 4.0f, 0.0));
	auto& cMat = cube.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::CUBE), 
		Material::Create(MaterialType::LIT));
	cMat.MaterialAsset->Set("u_HasColor", true);
	cMat.MaterialAsset->Set("u_Color", Vec3(0.212f, 0.271f, 0.741f));
	cMat.MaterialAsset->Set("u_HasAlbedoTexture", false);
	cMat.MaterialAsset->Set("u_HasNormalTexture", false);
	cMat.MaterialAsset->Set("u_HasSpecularTexture", false);
	
	/*auto grid = m_Scene.CreateEntity();
	grid.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::GRID), Material::Create(MaterialType::GRID));
	grid.AddComponent<TagComponent>("Grid");*/

	auto terrain = m_Scene.CreateEntity();
	auto& tMat = terrain.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::TERRAIN),
		Material::Create(MaterialType::LIT));
	tMat.MaterialAsset->Set("u_HasColor", true);
	tMat.MaterialAsset->Set("u_Color", Vec3(0.071f, 0.722f, 0.255f));
	tMat.MaterialAsset->Set("u_HasAlbedoTexture", false);
	tMat.MaterialAsset->Set("u_HasNormalTexture", false);
	tMat.MaterialAsset->Set("u_HasSpecularTexture", false);

	auto skyBox = m_Scene.CreateEntity();
	skyBox.AddComponent<SkyboxComponent>(std::make_shared<Skybox>(), 
		Material::Create(MaterialType::SKYBOX));
}

void GameLayer::OnEvent(Event& e)
{
	::EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<KeyPressed>([&](KeyPressed& key) 
	{
		if (key.getKey() == static_cast<int>(Key::Escape)) Application::Get().ShutDown();
	});

	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.OnEvent(e);
}

void GameLayer::OnUpdate(float dt)
{
	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.Update(dt);
}

void GameLayer::OnRender()
{
	RenderSystem::Render(m_Scene);	
}

void GameLayer::OnDetach() 
{
}

void GameLayer::OnUIRender() 
{
}

