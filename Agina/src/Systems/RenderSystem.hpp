#pragma once

#include <entt/entt.hpp>
#include <ECS/Components.hpp>

namespace Agina
{
	struct SceneData
	{
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::vec3 cameraPosition;
	};

	class RenderSystem
	{
	public:
		static void OnRender(entt::registry& reg)
		{
			auto cameraView = reg.view<CameraComponent>();
			for (auto [camEntity, cam] : cameraView.each())
			{
				SceneData data;
				data.cameraPosition = cam.camera.GetPosition();
				data.ViewMatrix = cam.camera.GetViewMatrix();
				data.ProjectionMatrix = cam.camera.GetProjectionMatrix();

				auto gridView = reg.view<GridComponent>();
				for (auto [e, g] : gridView.each())
					g.grid.Draw(data.ViewMatrix, data.ProjectionMatrix, data.cameraPosition);

				auto meshView = reg.view<MeshRenderer, Transform>();
				for (auto [e, m, t] : meshView.each())
				{
					glm::mat4 modelMatrix = t.GetModelMatrix();
					m.model.Draw(data, modelMatrix);
				}

				//auto skyView = reg.view<SkyBoxComponent>();
				//for (auto [e, s] : skyView.each())
				//	s.skyBox.Draw(data.ViewMatrix, data.ProjectionMatrix);
			}

		}

	};
}