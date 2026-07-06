#include "GameLayer.hpp"
#include <Renderer/Material.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Renderer.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Agina;

std::shared_ptr<Material> mat = nullptr;
std::shared_ptr<Mesh> mesh = nullptr;

void GameLayer::OnAttach()
{
	mat = Material::Create(MaterialType::UNLIT);
	mesh = Mesh::Create(MeshType::TRIANGLE);

	mat->Set("uColor", glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.5f, 0.1f, 1.0f));
	mat->Set("uTrans", transform);
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnEvent(Agina::Event& e)
{

}

void GameLayer::OnUpdate(float dt)
{

}

void GameLayer::OnRender()
{
	Renderer::Draw(*mesh, *mat);
}

void GameLayer::OnUIRender()
{

}
