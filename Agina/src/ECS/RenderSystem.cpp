#include "Components.hpp"
#include "RenderSystem.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Skybox.hpp> 
#include <Renderer/Model.hpp>  
#include <Renderer/Texture.hpp>
#include <Core/MathTypes.hpp>

namespace Agina {

	void RenderSystem::Render(Scene& scene)
	{
		auto cameraEntity = scene.FindEntityWithComponent<CameraComponent>();
		if (!cameraEntity.has_value()) return;
		auto& camera = cameraEntity->GetComponent<CameraComponent>().Cam;

		Vec3 lightPos(-10.0f, 20.0f, -5.0f);
		Vec3 lightTarget(0.0f, 0.0f, 0.0f);

		auto lightEntity = scene.FindEntityWithComponent<DirectionalLightComponent>();
		if (lightEntity.has_value())
		{
			const auto& light = lightEntity->GetComponent<DirectionalLightComponent>();
			lightPos = light.Position;
			lightTarget = light.Target;
		}

		Renderer::BeginShadowPass(lightPos, lightTarget);

		scene.Each<Transform, MeshComponent>([](Entity entity) {
			auto& transform = entity.GetComponent<Transform>();
			auto& meshComp = entity.GetComponent<MeshComponent>();

			if (meshComp.MeshAsset && meshComp.MaterialAsset)
			{
				Renderer::Draw(*meshComp.MeshAsset, *meshComp.MaterialAsset, transform);
			}
			});

		scene.Each<Transform, ModelComponent>([](Entity entity) {
			auto& transform = entity.GetComponent<Transform>();
			auto& modelComp = entity.GetComponent<ModelComponent>();

			if (modelComp.ModelAsset && modelComp.MaterialAsset)
			{
				Renderer::Draw(*modelComp.ModelAsset, *modelComp.MaterialAsset, transform);
			}
			});

		Renderer::EndShadowPass();

		Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::BeginScene(camera);

#if DEBUG_SHADOW_MAP
		Renderer::EndScene();
		return;
#endif

		scene.Each<Transform, MeshComponent>([](Entity entity) {
			auto& transform = entity.GetComponent<Transform>();
			auto& meshComp = entity.GetComponent<MeshComponent>();

			if (meshComp.MeshAsset && meshComp.MaterialAsset)
			{
				Renderer::Draw(*meshComp.MeshAsset, *meshComp.MaterialAsset, transform);
			}
			});

		scene.Each<Transform, ModelComponent>([](Entity entity) {
			auto& transform = entity.GetComponent<Transform>();
			auto& modelComp = entity.GetComponent<ModelComponent>();

			if (modelComp.ModelAsset && modelComp.MaterialAsset)
			{
				Renderer::Draw(*modelComp.ModelAsset, *modelComp.MaterialAsset, transform);
			}
			});

		auto skyBoxEntity = scene.FindEntityWithComponent<SkyboxComponent>();
		if (!skyBoxEntity.has_value()) return;
		auto& skyBoxComp = skyBoxEntity->GetComponent<SkyboxComponent>();
		skyBoxComp.skyBoxMaterialAsset->Bind();
		Renderer::DrawSkybox(skyBoxComp.skyBoxAsset);

		Renderer::EndScene();
	}
}

