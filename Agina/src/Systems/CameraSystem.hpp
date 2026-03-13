#pragma once

#include <entt/entt.hpp>
#include <ECS/Components.hpp>
#include <Core/Inputs/Events.hpp>
#include <Renderer/Renderer.hpp>

namespace Agina
{
	class CameraSystem
	{
	public:
		static void OnUpdate(entt::registry& reg, float dt)
		{
			auto view = reg.view<CameraComponent>();
			for (auto [e, c] : view.each()) c.camera.OnUpdate(dt);
		}

		static void onEvent(entt::registry& registry, Event& e)
		{
			auto view = registry.view<CameraComponent>();
			for (auto [entity, cameraComponent] : view.each())
				cameraComponent.camera.OnEvent(e);

			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowResized>([&](WindowResized& w)
				{
					Renderer::SetViewport(0, 0, w.GetNewWidth(), w.GetNewHeight());
					for (auto [entity, cameraComponent] : view.each())
						cameraComponent.camera.updateAspectRatio(w.GetNewWidth(), w.GetNewHeight());
				});
		}
	};
}