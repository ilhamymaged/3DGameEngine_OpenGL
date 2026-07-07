#include "RenderSystem.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Skybox.hpp> 
#include "Components.hpp"

namespace Agina {

	void RenderSystem::Render(entt::registry& registry)
	{
		const Camera* primaryCamera = nullptr;
		auto cameraView = registry.view<CameraComponent>();
		for (auto entity : cameraView) 
		{
			const auto& camComp = cameraView.get<CameraComponent>(entity);
			if (camComp.IsPrimary) 
			{
				primaryCamera = &camComp.Cam;
				break;
			}
		}

		if (!primaryCamera) return; 

		glm::vec3 lightPos(-10.0f, 20.0f, -5.0f);
		glm::vec3 lightTarget(0.0f, 0.0f, 0.0f);

		auto lightView = registry.view<DirectionalLightComponent>();
		if (!lightView.empty()) 
		{
			auto entity = lightView.front();
			const auto& light = lightView.get<DirectionalLightComponent>(entity);
			lightPos = light.Position;
			lightTarget = light.Target;
		}

		Renderer::BeginShadowPass(lightPos, lightTarget);
		registry.view<Transform, MeshComponent>().each([](const auto& transform, const auto& meshComp) 
			{
			if (meshComp.MeshAsset && meshComp.MaterialAsset) 
			{
				if (meshComp.DiffuseTextureAsset)
				{
					meshComp.DiffuseTextureAsset->Bind(0);
					meshComp.MaterialAsset->Set("u_HasTexture", true);
				}
				else
				{
					meshComp.MaterialAsset->Set("u_HasTexture", false);
				}
				Renderer::Draw(*meshComp.MeshAsset, *meshComp.MaterialAsset, transform);
			}
			});
		Renderer::EndShadowPass();

		Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::BeginScene(*primaryCamera);
		registry.view<Transform, MeshComponent>().each([](const auto& transform, const auto& meshComp) 
			{
			if (meshComp.MeshAsset && meshComp.MaterialAsset) 
			{
				Renderer::Draw(*meshComp.MeshAsset, *meshComp.MaterialAsset, transform);
			}
			});

		auto skyboxView = registry.view<SkyboxComponent>();
		if (!skyboxView.empty()) 
		{
			auto skyEntity = skyboxView.front();
			const auto& skyboxComp = skyboxView.get<SkyboxComponent>(skyEntity);
			
			if (skyboxComp.Enabled)
			{
				Renderer::DrawSkybox();
			}
		}

		Renderer::EndScene();
	}
}
