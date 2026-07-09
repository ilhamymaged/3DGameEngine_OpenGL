#include "Scene.hpp"
#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Model.hpp>
#include <Renderer/Transform.hpp>
#include <ECS/Components.hpp>
#include <Core/FileSystem/FileSystem.hpp>

using namespace Agina;
void Scene::OnCreate()
{
	auto lightEntity = m_Registry.create();
	m_Registry.emplace<DirectionalLightComponent>(
		lightEntity,
		glm::vec3(-10.0f, 20.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);

	auto skyEntity = m_Registry.create();
	m_Registry.emplace<SkyboxComponent>(skyEntity, true);
	m_Registry.emplace<TagComponent>(skyEntity, "SkyBox");

	auto triangleMesh = Mesh::Create(MeshType::TRIANGLE);
	auto sphereMesh = Mesh::Create(MeshType::SPHERE);
	auto gridMesh = Mesh::Create(MeshType::TERRAIN);

	auto triangle = m_Registry.create();
	m_Registry.emplace<Transform>(triangle, glm::vec3(2.0f, 1.0f, 0.0f));
	m_Registry.emplace<TagComponent>(triangle, "Triangle");
	auto& triMat = m_Registry.emplace<MeshComponent>(triangle, triangleMesh, Material::Create(MaterialType::LIT));
	triMat.MaterialAsset->Set("u_Color", glm::vec3(0.0f, 1.0f, 0.0f));
	triMat.MaterialAsset->Set("u_HasColor", true);

	auto player = m_Registry.create();
	m_Registry.emplace<Transform>(player, glm::vec3(0.0f, 2.0f, 0.0f));
	m_Registry.emplace<TagComponent>(player, "Player");
	auto& pMat = m_Registry.emplace<MeshComponent>(player, sphereMesh, Material::Create(MaterialType::LIT));
	pMat.MaterialAsset->Set("u_Color", glm::vec3(1.0f, 0.0f, 0.0f));
	pMat.MaterialAsset->Set("u_HasColor", true);

	auto floor = m_Registry.create();
	m_Registry.emplace<Transform>(floor, glm::vec3(0.0f));
	m_Registry.emplace<TagComponent>(floor, "Ground");
	auto& fMat = m_Registry.emplace<MeshComponent>(floor, gridMesh, Material::Create(MaterialType::LIT));
	fMat.MaterialAsset->Set("u_Color", glm::vec3(1.0f, 1.0f, 1.0f));
	fMat.MaterialAsset->Set("u_HasColor", true);
	std::string grassTexturePath = (FileSystem::AppAssets() / "textures/highqualitybrickn.jpg").string();
	fMat.DiffuseTextureAsset = std::make_shared<Texture>(grassTexturePath);

	auto backpackEntity = m_Registry.create();
	m_Registry.emplace<Transform>(backpackEntity, glm::vec3(0.0f, 2.0f, -2.0f));
	m_Registry.emplace<TagComponent>(backpackEntity, "BackPack");
	std::string backpackPath = (FileSystem::AppAssets() / "models/backpack/backpack.obj").string();
	auto backpackModel = std::make_shared<Model>(backpackPath);
	auto backpackMat = Material::Create(MaterialType::LIT);
	backpackMat->Set("u_Color", glm::vec3(1.0f, 1.0f, 1.0f));
	backpackMat->Set("u_HasColor", true);
	m_Registry.emplace<ModelComponent>(backpackEntity, backpackModel, backpackMat);
}

void Scene::OnDestroy()
{
}

