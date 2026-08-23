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

			collider.worldMin = transform.Position + collider.localMin * transform.Scale;
			collider.worldMax = transform.Position + collider.localMax * transform.Scale;
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
            //rb.acceleration += GRAVITY * rb.gravityScale;
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

                auto& otherCollider = otherEntity.GetComponent<BoxCollider>();

                if (!CheckAABBCollision(dynamicCollider, otherCollider))
                    return;

                float overlapX = std::min(dynamicCollider.worldMax.x, otherCollider.worldMax.x) - std::max(dynamicCollider.worldMin.x, otherCollider.worldMin.x);
                float overlapY = std::min(dynamicCollider.worldMax.y, otherCollider.worldMax.y) - std::max(dynamicCollider.worldMin.y, otherCollider.worldMin.y);
                float overlapZ = std::min(dynamicCollider.worldMax.z, otherCollider.worldMax.z) - std::max(dynamicCollider.worldMin.z, otherCollider.worldMin.z);

                const float kEpsilon = 0.001f;

                if (overlapX < overlapY && overlapX < overlapZ)
                {
                    float centerX1 = (dynamicCollider.worldMin.x + dynamicCollider.worldMax.x) * 0.5f;
                    float centerX2 = (otherCollider.worldMin.x + otherCollider.worldMax.x) * 0.5f;

                    dynamicTransform.Position.x += (centerX1 > centerX2) ? (overlapX + kEpsilon) : -(overlapX + kEpsilon);
                    dynamicRb.velocity.x = 0.0f;
                }
                else if (overlapZ < overlapX && overlapZ < overlapY)
                {
                    float centerZ1 = (dynamicCollider.worldMin.z + dynamicCollider.worldMax.z) * 0.5f;
                    float centerZ2 = (otherCollider.worldMin.z + otherCollider.worldMax.z) * 0.5f;

                    dynamicTransform.Position.z += (centerZ1 > centerZ2) ? (overlapZ + kEpsilon) : -(overlapZ + kEpsilon);
                    dynamicRb.velocity.z = 0.0f;
                }
                else 
                {
                    float centerY1 = (dynamicCollider.worldMin.y + dynamicCollider.worldMax.y) * 0.5f;
                    float centerY2 = (otherCollider.worldMin.y + otherCollider.worldMax.y) * 0.5f;

                    dynamicTransform.Position.y += (centerY1 > centerY2) ? (overlapY + kEpsilon) : -(overlapY + kEpsilon);

                    if (centerY1 > centerY2 && dynamicRb.velocity.y < 0.0f) {
                        dynamicRb.velocity.y = 0.0f;
                    }
                }
                dynamicCollider.worldMin = dynamicTransform.Position + (dynamicCollider.localMin * dynamicTransform.Scale);
                dynamicCollider.worldMax = dynamicTransform.Position + (dynamicCollider.localMax * dynamicTransform.Scale);
                });
            });
        UpdateColliders(scene);
    }
}