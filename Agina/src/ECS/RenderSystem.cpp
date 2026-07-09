#include "RenderSystem.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/Skybox.hpp> 
#include <Renderer/Model.hpp>  
#include <Renderer/Texture.hpp>
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
					Renderer::Draw(*meshComp.MeshAsset, *meshComp.MaterialAsset, transform);
				}
			});

		registry.view<Transform, ModelComponent>().each([](const auto& transform, const auto& modelComp)
			{
				if (modelComp.ModelAsset && modelComp.MaterialAsset)
				{
					const auto& meshes = modelComp.ModelAsset->GetMeshes();
					for (const auto& subMesh : meshes)
					{
						if (subMesh) 
						{
							Renderer::Draw(*subMesh, *modelComp.MaterialAsset, transform);
						}
					}
				}
			});

		Renderer::EndShadowPass();

		Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::BeginScene(*primaryCamera);

#if DEBUG_SHADOW_MAP
		Renderer::EndScene();
		return;
#endif

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

		registry.view<Transform, ModelComponent>().each([](const auto& transform, const auto& modelComp)
			{
				if (modelComp.ModelAsset && modelComp.MaterialAsset)
				{
					const auto& meshes = modelComp.ModelAsset->GetMeshes();
					const auto& textures = modelComp.ModelAsset->GetTextures();

					for (size_t i = 0; i < meshes.size(); i++)
					{
						if (!meshes[i]) continue;

						if (i < textures.size() && textures[i])
						{
							textures[i]->Bind(0);
							modelComp.MaterialAsset->Set("u_HasTexture", true);
						}
						else
						{
							modelComp.MaterialAsset->Set("u_HasTexture", false);
						}

						Renderer::Draw(*meshes[i], *modelComp.MaterialAsset, transform);
					}
				}
			});

		auto skyboxView = registry.view<SkyboxComponent>();
		if (!skyboxView.empty())
		{
			auto skyEntity = skyboxView.front();
			const auto& skyboxComp = skyboxView.get<SkyboxComponent>(skyEntity);

			if (skyboxComp.Enabled) Renderer::DrawSkybox();
		}
		Renderer::EndScene();
	}
}
