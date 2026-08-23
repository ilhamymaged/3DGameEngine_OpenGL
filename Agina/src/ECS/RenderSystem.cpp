#include "Components.hpp"
#include "RenderSystem.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Skybox.hpp> 
#include <Renderer/Model.hpp>  
#include <Core/MathTypes.hpp>

namespace Agina {

	static void RenderShadowPass(Scene& scene)
	{
		Renderer::BeginShadowPass();

		scene.Each<Transform, MeshComponent>([](Entity entity) {
			auto& transform = entity.GetComponent<Transform>();
			auto& meshComp = entity.GetComponent<MeshComponent>();

			if (meshComp.MeshAsset && meshComp.MaterialAsset)
				Renderer::Submit(*meshComp.MeshAsset, *meshComp.MaterialAsset, transform);
			});

		Renderer::EndShadowPass();
	}

	static void RenderMainPass(Scene& scene, const Camera& camera)
	{
		Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::BeginScene(camera);

		scene.Each<Transform, MeshComponent>([](Entity entity) {
			auto& transform = entity.GetComponent<Transform>();
			auto& meshComp = entity.GetComponent<MeshComponent>();

			if (meshComp.MeshAsset && meshComp.MaterialAsset)
			{
				Renderer::Submit(*meshComp.MeshAsset, *meshComp.MaterialAsset, transform);
			}
			});
		
		auto skyBoxEntity = scene.FindEntityWithComponent<SkyboxComponent>();
		if (skyBoxEntity.has_value())
		{
			auto& skyBoxComp = skyBoxEntity->GetComponent<SkyboxComponent>();
			Renderer::SubmitSkyBox(skyBoxComp.skyBoxAsset, *skyBoxComp.skyBoxMaterialAsset);
		}

		scene.Each<BoxCollider>([](Entity entity) 
			{
			auto& collider = entity.GetComponent<BoxCollider>();

			Vec4 color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
			if (entity.HasComponent<Rigidbody>() && !entity.GetComponent<Rigidbody>().isStatic) 
			{
				color = Vec4(1.0f, 0.3f, 0.3f, 1.0f); 
			}
			Renderer::SubmitDebugBox(collider.worldMin, collider.worldMax, color);
			});

		Renderer::EndScene();
	}

	void RenderSystem::RenderToTarget(Scene& scene, const std::shared_ptr<Framebuffer>& target)
	{
		if (!target) return;

		target->Bind();
		Render(scene, target);
		target->Unbind();
	}

	void RenderSystem::Render(Scene& scene, const std::shared_ptr<Framebuffer>& target)
	{
		auto cameraEntity = scene.FindEntityWithComponent<CameraComponent>();
		if (!cameraEntity.has_value()) return;
		auto IsPrimary = cameraEntity->GetComponent<CameraComponent>().IsPrimary;
		if (!IsPrimary) return;
		auto& camera = cameraEntity->GetComponent<CameraComponent>().Cam;

		Vec3 lightPos(-10.0f, 20.0f, -5.0f);
		Vec3 lightTarget(0.0f, 0.0f, 0.0f);

		auto lightEntity = scene.FindEntityWithComponent<DirectionalLightComponent>();
		if (lightEntity.has_value())
		{
			const auto& light = lightEntity->GetComponent<DirectionalLightComponent>();
			if (lightEntity->HasComponent<Transform>())
			{
				const auto& t = lightEntity->GetComponent<Transform>();
				lightPos = t.Position;
			}

			lightTarget = light.Target;
		}

		Renderer::UpdateLightData(lightPos, lightTarget);
		if (Renderer::GetShadowsEnabled())
		{
			target->Unbind();
			RenderShadowPass(scene);
		}
		if (target) target->Bind();
		RenderMainPass(scene, camera);
	}
}