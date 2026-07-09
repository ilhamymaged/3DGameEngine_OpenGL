#include <ECS/PhysicsSystem.hpp>
#include <ECS/Components.hpp>
#include <glm/glm.hpp>

#define GRAVITY glm::vec3 (0.0f, -9.81f, 0.0f)

namespace Agina  {
	
	void PhysicsSystem::Update(entt::registry& registry, float dt)
	{

	}

	static bool Intersects(const AABB& a, const AABB& b)
	{
		return
			a.Min.x <= b.Max.x && a.Max.x >= b.Min.x &&
			a.Min.y <= b.Max.y && a.Max.y >= b.Min.y &&
			a.Min.z <= b.Max.z && a.Max.z >= b.Min.z;
	}

	static AABB GetWorldAABB(const Transform& transform, const MeshComponent& mesh)
	{
		const AABB& local = mesh.MeshAsset->GetAABB();

		return {
			local.Min * transform.Scale + transform.Position,
			local.Max * transform.Scale + transform.Position
		};
	}
}
