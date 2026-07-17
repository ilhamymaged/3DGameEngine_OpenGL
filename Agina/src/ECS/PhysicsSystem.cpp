#include <ECS/PhysicsSystem.hpp>
#include <ECS/Components.hpp>
#include <Core/MathTypes.hpp>
#include <Core/Logger.hpp>

#define GRAVITY Vec3(0.0f, -9.81f, 0.0f)

namespace Agina {

	static void UpdateColliders(Scene& scene)
	{
		scene.Each<Transform, BoxCollider>([](Entity e) {
			auto& transform = e.GetComponent<Transform>();
			auto& collider = e.GetComponent<BoxCollider>();

			collider.worldMin = transform.Position + collider.localMin;
			collider.worldMax = transform.Position + collider.localMax;
			});
	}

	static bool CheckAABBCollision(const BoxCollider& a, const BoxCollider& b)
	{
		return (a.worldMin.x <= b.worldMax.x && a.worldMax.x >= b.worldMin.x) &&
			(a.worldMin.y <= b.worldMax.y && a.worldMax.y >= b.worldMin.y) &&
			(a.worldMin.z <= b.worldMax.z && a.worldMax.z >= b.worldMin.z);
	}


	void PhysicsSystem::Update(Scene& scene, float dt)
	{
		scene.Each<Transform, Rigidbody>([dt](Entity e) {
			auto& transform = e.GetComponent<Transform>();
			auto& rb = e.GetComponent<Rigidbody>();

			if (rb.isStatic) return;

			float invMass = rb.mass > 0.0f ? (1.0f / rb.mass) : 0.0f;

			rb.acceleration += rb.forceAccumulator * invMass;

			rb.acceleration += GRAVITY * rb.gravityScale;
			rb.velocity += rb.acceleration * dt;

			transform.Position += rb.velocity * dt;

			rb.acceleration = Vec3(0.0f);
			rb.forceAccumulator = Vec3(0.0f);
			});

		UpdateColliders(scene);

		scene.Each<Transform, Rigidbody, BoxCollider>([&scene](Entity dynamicEntity) {
			auto& dynamicRb = dynamicEntity.GetComponent<Rigidbody>();
			if (dynamicRb.isStatic) return;

			auto& dynamicTransform = dynamicEntity.GetComponent<Transform>();
			auto& dynamicCollider = dynamicEntity.GetComponent<BoxCollider>();

			scene.Each<Transform, Rigidbody, BoxCollider>([&](Entity otherEntity) {
				if (dynamicEntity == otherEntity) return;

				auto& otherRb = otherEntity.GetComponent<Rigidbody>();
				auto& otherCollider = otherEntity.GetComponent<BoxCollider>();

				if (!CheckAABBCollision(dynamicCollider, otherCollider))
					return;

				// Compute penetration depth on Y axis
				float overlapY = std::min(dynamicCollider.worldMax.y, otherCollider.worldMax.y) -
					std::max(dynamicCollider.worldMin.y, otherCollider.worldMin.y);

				if (overlapY <= 0.0f) return;

				const float kEpsilon = 0.001f;

				if (dynamicRb.velocity.y < 0.0f)
				{
					dynamicTransform.Position.y += overlapY + kEpsilon;
				}
				else
				{
					dynamicTransform.Position.y -= overlapY + kEpsilon;
				}

				dynamicRb.velocity.y = 0.0f;
				dynamicRb.acceleration.y = 0.0f;
				dynamicRb.forceAccumulator.y = 0.0f;

				dynamicCollider.worldMin = dynamicTransform.Position + dynamicCollider.localMin;
				dynamicCollider.worldMax = dynamicTransform.Position + dynamicCollider.localMax;

				});
			});

		UpdateColliders(scene);
	}
}