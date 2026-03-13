#pragma once

#include <Scene/Scene.hpp>
#include <imgui.h>

namespace Agina
{
	void DrawSceneHierarchy(Scene* scene, Entity& e)
	{
        ImGui::Begin("Scene Hierarchy");
        if (scene)
        {
            auto view = scene->m_Registry.view<TagComponent>();
            ImGui::Separator();
            for (auto entityHandle : view)
            {
                auto& tag = view.get<TagComponent>(entityHandle).tag;
                if (ImGui::Selectable(tag.c_str()))
                    e = { entityHandle, scene};
            }
        }

        ImGui::End();
	}
}