#include "GameLayer.hpp"
#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Transform.hpp>
#include <Application.hpp>
#include <Core/Inputs/KeyMappings.hpp>
#include <Renderer/Camera.hpp>
#include <Core/Inputs/Inputs.hpp>
#include <Renderer/CubemapTexture.hpp> 
#include <ECS/RenderSystem.hpp>
#include <ECS/Components.hpp>
#include <Core/FileSystem/FileSystem.hpp>
#include <UI/UI.hpp>

using namespace Agina;

entt::registry m_Registry;
entt::entity cameraEntity;

void GameLayer::OnAttach()
{	
	cameraEntity = m_Registry.create();
	auto& camComp = m_Registry.emplace<CameraComponent>(cameraEntity);
	camComp.Cam = Camera(glm::vec3(0.0f, 2.0f, 5.0f));
	camComp.IsPrimary = true;

	auto lightEntity = m_Registry.create();
	m_Registry.emplace<DirectionalLightComponent>(
		lightEntity,
		glm::vec3(-10.0f, 20.0f, -5.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f)     
	);

	auto skyEntity = m_Registry.create();
	m_Registry.emplace<SkyboxComponent>(skyEntity, true);

	auto triangleMesh = Mesh::Create(MeshType::TRIANGLE);
	auto sphereMesh = Mesh::Create(MeshType::SPHERE);
	auto gridMesh = Mesh::Create(MeshType::TERRAIN); 

	auto triangle = m_Registry.create();
	m_Registry.emplace<Transform>(triangle, glm::vec3(2.0f, 1.0f, 0.0f));
	auto& triMat = m_Registry.emplace<MeshComponent>(triangle, triangleMesh, Material::Create(MaterialType::LIT));
	triMat.MaterialAsset->Set("u_Color", glm::vec3(0.0f, 1.0f, 0.0f));
	triMat.MaterialAsset->Set("u_HasColor", true);

	auto player = m_Registry.create();
	m_Registry.emplace<Transform>(player, glm::vec3(0.0f, 2.0f, 0.0f));
	auto& pMat = m_Registry.emplace<MeshComponent>(player, sphereMesh, Material::Create(MaterialType::LIT));
	pMat.MaterialAsset->Set("u_Color", glm::vec3(1.0f, 0.0f, 0.0f));
	pMat.MaterialAsset->Set("u_HasColor", true);

	auto floor = m_Registry.create();
	m_Registry.emplace<Transform>(floor, glm::vec3(0.0f));
	auto& fMat = m_Registry.emplace<MeshComponent>(floor, gridMesh, Material::Create(MaterialType::LIT));
	fMat.MaterialAsset->Set("u_Color", glm::vec3(1.0f, 1.0f, 1.0f));
	fMat.MaterialAsset->Set("u_HasColor", true);
	std::string grassTexturePath = (FileSystem::AppAssets() / "textures/highqualitybrickn.jpg").string();
	fMat.DiffuseTextureAsset = std::make_shared<Texture>(grassTexturePath);
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

	auto& camComp = m_Registry.get<CameraComponent>(cameraEntity);
	camComp.Cam.OnEvent(e);
}

void GameLayer::OnUpdate(float dt)
{
	m_Registry.get<CameraComponent>(cameraEntity).Cam.Update(dt);
}

void GameLayer::OnRender()
{
	RenderSystem::Render(m_Registry);
}

void GameLayer::OnDetach() {}
void GameLayer::OnUIRender()
{
	UI::BeginWindow("ECS System Controller Dashboard");

	auto skyboxView = m_Registry.view<SkyboxComponent>();
	if (!skyboxView.empty())
	{
		auto skyEntity = skyboxView.front();
		auto& skyboxComp = skyboxView.get<SkyboxComponent>(skyEntity);

		UI::Checkbox("Enable Environmental Skybox Background", &skyboxComp.Enabled);
	}

	UI::Spacing(5.0f);
	UI::EndWindow();
}
