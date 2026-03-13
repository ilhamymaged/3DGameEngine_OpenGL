#pragma once

#include <ECS/Entity.hpp>
#include <imgui.h>
#include "SceneState.hpp"

namespace Agina
{
    void DrawInspector(const glm::vec2& viewPortSize, SceneState s, Entity& e,
        Scene*& scene)
    {
        ImGui::Begin("Inspector");
        if (e)
        {
            if (e.HasComponent<TagComponent>())
            {
                auto& tag = e.GetComponent<TagComponent>().tag;
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strncpy(buffer, tag.c_str(), sizeof(buffer));

                if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                    tag = std::string(buffer);
            }

            ImGui::Separator();
            if (e.HasComponent<Transform>())
            {
                auto& transform = e.GetComponent<Transform>();
                ImGui::DragFloat3("Translation", &transform.position.x, 0.1f);
                ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.1f);
                ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
            }

            ImGui::Separator();
            if (ImGui::Button("Delete Entity")) scene->DestroyEntity(e);
        }
        else
        {

            ImGui::Text("ViewPortSize: {%d,%d}", viewPortSize.x, viewPortSize.y);
            ImGui::SameLine();
            
            ImGui::Text("ViewPortMode: {%d}", s);

            ImGui::Separator();
            ImGui::Text("No Entity Selected");
        }
        ImGui::End();
	}
}
