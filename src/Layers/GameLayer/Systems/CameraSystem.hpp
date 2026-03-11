#pragma once

#include <entt/entt.hpp>
#include <Layers/GameLayer/ECS/Components.hpp>
#include <Application/Inputs/Events.hpp>
#include <Application/Renderer/Renderer.hpp>

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

		dispatcher.Dispatch<KeyPressed>([&](KeyPressed& k)
			{
				if (k.getKey() == GLFW_KEY_P)
				{
					for (auto [entity, cameraComponent] : view.each())
						cameraComponent.camera.SetSpeed(cameraComponent.camera.GetSpeed() + 1.0f);
				}

				if (k.getKey() == GLFW_KEY_K)
				{
					for (auto [entity, cameraComponent] : view.each())
						std::cout << cameraComponent.camera.GetSpeed() << std::endl;
				}


				if (k.getKey() == GLFW_KEY_O)
				{
					for (auto [entity, cameraComponent] : view.each())
						cameraComponent.camera.SetSpeed(cameraComponent.camera.GetSpeed() - 1.0f);
				}
			});
	}
};
